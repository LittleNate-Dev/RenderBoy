#include "Core.h"

// Statics and Consts
// Use these bools to open or close a window
namespace rbcore
{
	Settings SETTINGS;
	ImGui::FileBrowser FILE_BROWSER;
	Load_Type LOAD_TYPE = NO_FILE;
	std::string FILEPATH_BUFFER = "";
	glm::vec3 NORMAL_COLOR = glm::vec3(1.0f);
	float NORMAL_MAGNITUDE = 1.0f;
	bool RELOAD_FONT = false;
	const char* currentModelInfo = nullptr;
	const char* currentModelScene = nullptr;
	const char* currentPointLight = nullptr;
	const char* currentSpotLight = nullptr;
	const char* currentDirLight = nullptr;
	bool IS_RENDERBOY_OPENED = true;
	bool IS_ABOUT_OPENED = false;
	bool IS_INFO_OPENED = false;
	bool IS_SETTINGS_OPENED = false;
	bool IS_WARNING_OPENED = false;
	bool IS_LOADING_OPENED = false;
	bool IS_SCENE_OPENED = false;
	bool IS_MODELS_OPENED = false;
	bool IS_CAMERA_OPENED = false;
	bool IS_LIGHTS_OPENED = false;
	std::string POPUP_MSG = "";
	const std::vector<std::string> FILE_TYPE({
		"gltf", "obj", "jpg", "png"
		});
	const std::vector<std::string> FILE_TYPE_ASSIMP({
		"3mf", "blend", "3ds", "ase", "gltf", "fbx",
		"ply", "smd", "vta", "x", "raw", "dxf", "stl",
		"obj"
		});
	const std::vector<std::string> FONT_STYLE({
		"Open Sans",
		"Roboto",
		"Raleway",
		"Courier",
		"Rubik"
		});
}
// Use these statics to set ui style
namespace ImGui
{
	unsigned int COLOR_HIGHTLIGHT_STYLE = 0xC9252D;
	unsigned int COLOR_WARNING_SYTLE = 0xC9252D;
}

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		spdlog::error("ERROR::OPENGL (" + std::to_string(error) + ")");
		spdlog::error(function + (std::string)" " + file + ":" + std::to_string(line));
		return false;
	}
	return true;
}

namespace rbcore
{
	glm::vec2 GetRenderResolution()
	{
		int width = (int)(rbcore::SETTINGS.Width * rbcore::SETTINGS.Resolution);
		int height = (int)(rbcore::SETTINGS.Height * rbcore::SETTINGS.Resolution);
		glm::vec2 resolution = glm::vec2(width, height);
		return resolution;
	}

	void ShowWarningMsg(std::string msg)
	{
		IS_WARNING_OPENED = true;
		POPUP_MSG = msg;
	}

	bool CheckFileFormat(std::string fileType)
	{
		if (std::find(FILE_TYPE.begin(), FILE_TYPE.end(), fileType) != FILE_TYPE.end())
		{
			return true;
		}
		return false;
	}

	bool CheckFileFormatAssimp(std::string fileType)
	{
		if (std::find(FILE_TYPE_ASSIMP.begin(), FILE_TYPE_ASSIMP.end(), fileType) != FILE_TYPE_ASSIMP.end())
		{
			return true;
		}
		return false;
	}

	std::string GetSettingValue(std::string setting)
	{
		std::string value;
		int endIndex = 0;
		char separator = ' ';
		for (unsigned int i = 0; i < setting.size(); i++)
		{
			if (setting[i] == separator || i == setting.size())
			{
				endIndex = i + 1;
				value.append(setting, endIndex, setting.size() - endIndex);
				break;
			}
		}
		return value;
	}

	std::vector<std::string> GetSceneValue(std::string scene)
	{
		std::vector<std::string> values;
		std::istringstream stream(scene);
		std::string value;
		int skip = 0;
		while (std::getline(stream, value, ' '))
		{
			if (skip)
			{
				values.push_back(value);
			}
			skip++;
		}
		return values;
	}

	std::string GetFileDirectory(std::string filepath)
	{
		std::string::size_type iPos = (filepath.find_last_of('\\') + 1) == 0 ? filepath.find_last_of('/') + 1 : filepath.find_last_of('\\') + 1;
		std::string directory = filepath.substr(0, iPos);
		return directory;
	}

	std::string GetFileFormat(std::string filepath)
	{
		std::string fileType = filepath.substr(filepath.find_last_of('.') + 1);
		return fileType;
	}

	std::string GetFileName(std::string filepath)
	{
		std::string::size_type iPos = filepath.find_last_of('\\') + 1;
		std::string fileName = filepath.substr(iPos, filepath.length() - iPos);
		return fileName;
	}

	std::string GetFileNameNoSuffix(std::string filepath)
	{
		std::string::size_type iPos = filepath.find_last_of('\\') + 1;
		std::string fileName = filepath.substr(iPos, filepath.length() - iPos);
		fileName = fileName.substr(0, fileName.rfind("."));
		return fileName;
	}

	void SetUiStyle(UI_Style style)
	{
		ImGui::COLOR_HIGHTLIGHT_STYLE = 0xC9252D;
		ImGui::ResetStyleColors();
		switch (style)
		{
		case DEFAULT_LIGHT:
			ImGui::StyleColorsLight();
			break;
		case DEFAULT_DARK:
			ImGui::StyleColorsDark();
			break;
		case SPECTRUM:
			ImGui::StyleColorsSpectrum();
			break;
		}
	}

	std::string GetFontStylePath(std::string style)
	{
		if (std::find(FONT_STYLE.begin(), FONT_STYLE.end(), style) != FONT_STYLE.end())
		{
			return FONTS_FILEPATH + style + ".ttf";
		}
		return "";
	}

	int GetFontStyleIndex(std::string style)
	{
		return std::find(FONT_STYLE.begin(), FONT_STYLE.end(), style) - FONT_STYLE.begin();
	}

	float GetWidgetWidthCoefficient()
	{
		float coefficient = (float)rbcore::SETTINGS.FontSize / 17.0f;
		return coefficient;
	}

	glm::mat4 GetRodrigue(glm::vec4 axis, float angle)
	{
		glm::mat3 part1 = glm::cos(glm::radians(angle)) * glm::mat3(1.0f);
		glm::mat3 nnt = glm::mat3(
			axis.x * axis.x, axis.x * axis.y, axis.x * axis.z,
			axis.x * axis.y, axis.y * axis.y, axis.y * axis.z,
			axis.x * axis.z, axis.y * axis.z, axis.z * axis.z
		);
		glm::mat3 part2 = (1 - glm::cos(glm::radians(angle))) * nnt;
		glm::mat3 part3 = glm::mat3(
			0.0f, -axis.z, axis.y,
			axis.z, 0.0f, -axis.x,
			-axis.y, axis.x, 0.0f
		);
		part3 = glm::sin(glm::radians(angle)) * part3;
		glm::mat3 rodrigueMat3 = part1 + part2 + part3;
		glm::mat4 rodrigue;
		rodrigue[0] = glm::vec4(rodrigueMat3[0], 0.0f);
		rodrigue[1] = glm::vec4(rodrigueMat3[1], 0.0f);
		rodrigue[2] = glm::vec4(rodrigueMat3[2], 0.0f);
		rodrigue[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		return rodrigue;
	}

	glm::mat4 GetViewPortMatrix(unsigned int width, unsigned int height)
	{
		glm::mat4 viewportMat = glm::mat4(
			(float)width / 2.0f, 0.0f, 0.0f, (float)width / 2.0f,
			0.0f, (float)height / 2.0f, 0.0f, (float)height / 2.0f,
			0.0f, 0.0f, 0.5f, 0.5f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		return viewportMat;
	}

	glm::vec3 GetAttenuationValues(float range)
	{
		glm::vec3 clq = glm::vec3(1.0f, 0.7f, 1.8f);
		if (range <= 7.0f)
		{
			clq = glm::vec3(1.0f, 0.7f, 1.8f);
		}
		else if (range <= 13.0f)
		{
			clq = glm::vec3(1.0f, 0.35f, 0.44f);
		}
		else if (range <= 20.0f)
		{
			clq = glm::vec3(1.0f, 0.22f, 0.2f);
		}
		else if (range <= 32.0f)
		{
			clq = glm::vec3(1.0f, 0.14f, 0.07f);
		}
		else if (range <= 50.0f)
		{
			clq = glm::vec3(1.0f, 0.09f, 0.032f);
		}
		else if (range <= 65.0f)
		{
			clq = glm::vec3(1.0f, 0.07f, 0.017f);
		}
		else if (range <= 100.0f)
		{
			clq = glm::vec3(1.0f, 0.045f, 0.0075f);
		}
		else if (range <= 160.0f)
		{
			clq = glm::vec3(1.0f, 0.027f, 0.0028f);
		}
		else if (range <= 200.0f)
		{
			clq = glm::vec3(1.0f, 0.022f, 0.0019f);
		}
		else if (range <= 325.0f)
		{
			clq = glm::vec3(1.0f, 0.014f, 0.0007f);
		}
		else if (range <= 600.0f)
		{
			clq = glm::vec3(1.0f, 0.007f, 0.0002f);
		}
		else if (range <= 3250.0f)
		{
			clq = glm::vec3(1.0f, 0.0014f, 0.000007f);
		}
		else
		{
			clq = glm::vec3(1.0f, 0.0009f, 0.000004f);
		}
		return clq;
	}
}

namespace ImGui
{

	void CenterAlignWidget(std::string text)
	{
		float windowWidth = ImGui::GetWindowSize().x;
		float textWidth = ImGui::CalcTextSize(text.c_str()).x;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	}

	void CenterAlignWidget(float width)
	{
		float windowWidth = ImGui::GetWindowSize().x;
		ImGui::SetCursorPosX((windowWidth - width) * 0.5f);
	}

	void CenterAlignWidget(std::string text, float width)
	{
		float windowWidth = ImGui::GetWindowSize().x;
		float widgetWidth = ImGui::CalcTextSize(text.c_str()).x + width;
		ImGui::SetCursorPosX((windowWidth - widgetWidth) * 0.5f);
	}

	void LabelColored(std::string text, unsigned int color)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ColorConvertU32ToFloat4(Color(color)));
		ImGui::Text(text.c_str()); ImGui::SameLine();
		ImGui::PopStyleColor(1);
	}

	void LabelHighlighted(std::string text)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ColorConvertU32ToFloat4(Color(COLOR_HIGHTLIGHT_STYLE)));
		ImGui::Text(text.c_str()); ImGui::SameLine();
		ImGui::PopStyleColor(1);
	}

	void TextColored(std::string text, unsigned int color)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ColorConvertU32ToFloat4(Color(color)));
		ImGui::Text(text.c_str());
		ImGui::PopStyleColor(1);
	}

	void TextHighlighted(std::string text)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ColorConvertU32ToFloat4(Color(COLOR_HIGHTLIGHT_STYLE)));
		ImGui::Text(text.c_str());
		ImGui::PopStyleColor(1);
	}

	unsigned int Color(unsigned int c) 
	{
		const short a = 0xFF;
		const short r = (c >> 16) & 0xFF;
		const short g = (c >> 8) & 0xFF;
		const short b = (c >> 0) & 0xFF;
		return(a << 24)
			| (r << 0)
			| (g << 8)
			| (b << 16);
	}

	void ResetStyleColors()
	{
		ImGui::GetStyle() = ImGuiStyle();
	}

	void StyleColorsSpectrum()
	{
		
		ImGuiStyle* style = &ImGui::GetStyle();
		style->GrabRounding = 4.0f;
		style->FrameRounding = 3.0f;
		style->WindowRounding = 8.0f;
		style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style->ItemInnerSpacing = ImVec2(10.0f, 0.5f);

		ImGui::COLOR_HIGHTLIGHT_STYLE = Color(0x268E6C);
		ImGui::COLOR_WARNING_SYTLE = Color(0xC9252D);
		ImVec4* colors = style->Colors;
		colors[ImGuiCol_Text] = ColorConvertU32ToFloat4(Color(0x4B4B4B)); // text on hovered controls is gray900
		colors[ImGuiCol_TextDisabled] = ColorConvertU32ToFloat4(Color(0xB3B3B3));
		colors[ImGuiCol_WindowBg] = ColorConvertU32ToFloat4(Color(0xEAEAEA));
		colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		colors[ImGuiCol_PopupBg] = ColorConvertU32ToFloat4(Color(0xFFFFFF)); // not sure about this. Note: applies to tooltips too.
		colors[ImGuiCol_Border] = ColorConvertU32ToFloat4(Color(0xE1E1E1));
		colors[ImGuiCol_BorderShadow] = ColorConvertU32ToFloat4(Color(0x00000000)); // We don't want shadows. Ever.
		colors[ImGuiCol_FrameBg] = ColorConvertU32ToFloat4(Color(0xFAFAFA)); // this isnt right, spectrum does not do this, but it's a good fallback
		colors[ImGuiCol_FrameBgHovered] = ColorConvertU32ToFloat4(Color(0xFFFFFF));
		colors[ImGuiCol_FrameBgActive] = ColorConvertU32ToFloat4(Color(0xEAEAEA));
		colors[ImGuiCol_TitleBg] = ColorConvertU32ToFloat4(Color(0xE1E1E1)); // those titlebar values are totally made up, spectrum does not have this.
		colors[ImGuiCol_TitleBgActive] = ColorConvertU32ToFloat4(Color(0xEAEAEA));
		colors[ImGuiCol_TitleBgCollapsed] = ColorConvertU32ToFloat4(Color(0xCACACA));
		colors[ImGuiCol_MenuBarBg] = ColorConvertU32ToFloat4(Color(0xF5F5F5));
		colors[ImGuiCol_ScrollbarBg] = ColorConvertU32ToFloat4(Color(0xF5F5F5)); // same as regular background
		colors[ImGuiCol_ScrollbarGrab] = ColorConvertU32ToFloat4(Color(0xCACACA));
		colors[ImGuiCol_ScrollbarGrabHovered] = ColorConvertU32ToFloat4(Color(0x8E8E8E));
		colors[ImGuiCol_ScrollbarGrabActive] = ColorConvertU32ToFloat4(Color(0x707070));
		colors[ImGuiCol_CheckMark] = ColorConvertU32ToFloat4(Color(0x1473E6));
		colors[ImGuiCol_SliderGrab] = ColorConvertU32ToFloat4(Color(0x707070));
		colors[ImGuiCol_SliderGrabActive] = ColorConvertU32ToFloat4(Color(0x4B4B4B));
		colors[ImGuiCol_Button] = ColorConvertU32ToFloat4(Color(0xB3B3B3)); // match default button to Spectrum's 'Action Button'.
		colors[ImGuiCol_ButtonHovered] = ColorConvertU32ToFloat4(Color(0xFFFFFF));
		colors[ImGuiCol_ButtonActive] = ColorConvertU32ToFloat4(Color(0xEAEAEA));
		colors[ImGuiCol_Header] = ColorConvertU32ToFloat4(Color(0x2680EB));
		colors[ImGuiCol_HeaderHovered] = ColorConvertU32ToFloat4(Color(0x1473E6));
		colors[ImGuiCol_HeaderActive] = ColorConvertU32ToFloat4(Color(0x0D66D0));
		colors[ImGuiCol_Separator] = ColorConvertU32ToFloat4(Color(0xCACACA));
		colors[ImGuiCol_SeparatorHovered] = ColorConvertU32ToFloat4(Color(0x8E8E8E));
		colors[ImGuiCol_SeparatorActive] = ColorConvertU32ToFloat4(Color(0x707070));
		colors[ImGuiCol_ResizeGrip] = ColorConvertU32ToFloat4(Color(0xCACACA));
		colors[ImGuiCol_ResizeGripHovered] = ColorConvertU32ToFloat4(Color(0x8E8E8E));
		colors[ImGuiCol_ResizeGripActive] = ColorConvertU32ToFloat4(Color(0x707070));
		colors[ImGuiCol_PlotLines] = ColorConvertU32ToFloat4(Color(0x2680EB));
		colors[ImGuiCol_PlotLinesHovered] = ColorConvertU32ToFloat4(Color(0x0D66D0));
		colors[ImGuiCol_PlotHistogram] = ColorConvertU32ToFloat4(Color(0x2680EB));
		colors[ImGuiCol_PlotHistogramHovered] = ColorConvertU32ToFloat4(Color(0x0D66D0));
		colors[ImGuiCol_TextSelectedBg] = ColorConvertU32ToFloat4((Color(0x2680EB) & 0x00FFFFFF) | 0x33000000);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ColorConvertU32ToFloat4((Color(0x2C2C2C) & 0x00FFFFFF) | 0x0A000000);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.35f);
	}
}