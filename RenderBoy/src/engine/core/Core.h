/*
* Description:
*	This header file contains every basic header file that is needed in this program, to keep things simple, you only need to include this header file instead of bunch of shits
*/

#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <Windows.h>

#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imfilebrowser.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <spdlog/spdlog.h>

#include <freeimage/FreeImage.h>

#define APP_VERSION "ver 0.5.0"
#define GIT_REPO "https://github.com/LittleNate-Dev/RenderBoy"
#define SCENE_FILEPATH "saves/scene/"
#define SETTING_FILEPATH "data/config/settings.config"
#define LOGGER_FILEPATH "res/logger/RenderBoy_Logger.txt"
#define FONTS_FILEPATH "res/fonts/"
#define SCREENSHOT_FILEPATH "saves/screenshot/"
#define UV_MAP_FILEPATH "res/checkermap/CheckerMap.png"
#define SHADER_OPENGL "data/shaders/opengl/"
#define SHADER_OPENGL_WIREFRAME "data/shaders/opengl/draw_mode/WIREFRAME.glsl"
#define SHADER_OPENGL_DEPTH "data/shaders/opengl/draw_mode/DEPTH.glsl"
#define SHADER_OPENGL_UVSET "data/shaders/opengl/draw_mode/UVSET.glsl"
#define SHADER_OPENGL_POINTCLOUD "data/shaders/opengl/draw_mode/POINTCLOUD.glsl"
#define SHADER_OPENGL_SKYBOX "data/shaders/opengl/others/SKYBOX.glsl"
#define SHADER_OPENGL_NORMAL "data/shaders/opengl/others/NORMAL.glsl"
#define SHADER_OPENGL_LIGHTCUBE "data/shaders/opengl/others/LIGHTCUBE.glsl"
#define SHADER_OPENGL_SHADOW_POINT "data/shaders/opengl/shadow/POINTLIGHT.glsl"
#define SHADER_OPENGL_SHADOW_SPOT "data/shaders/opengl/shadow/SPOTLIGHT.glsl"
#define SHADER_OPENGL_SHADOW_DIR "data/shaders/opengl/shadow/DIRLIGHT.glsl"
#define MAX_FOV 120
#define MIN_FOV 40

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

// Enums
enum Core
{
	OPENGL = 0,
	VULKAN = 1,
	DIRECTX = 2
};

enum UI_Style
{
	DEFAULT_LIGHT = 0,
	DEFAULT_DARK = 1,
	SPECTRUM = 2
};

enum Load_Type
{
	NO_FILE = 0,
	RESET_SCENE = 1,
	ADD_MODEL = 2,
	LOAD_SKYBOX = 3
};

enum Light_Type
{
	POINT_LIGHT = 0,
	SPOT_LIGHT = 1,
	DIRECTIONAL_LIGHT = 2
};

enum Draw_Mode
{
	DEFAULT = 0,
	WIREFRAME = 1,
	POINTCLOUD = 2,
	UVSET = 3,
	DEPTH = 4
};

enum Render_Mode
{
	NOTEX_NOCOLOR = 0,
	NOTEX_HASCOLOR = 1,
	HASRTEX_ALLTEX_ALBEDO = 3,
	HASTEX_ALLTEX_BLINN = 4,
	HASTEX_ALLTEX_PBR_3 = 5,
	HASTEX_ALLTEX_PBR_4 = 5,
	HASTEX_PARTTEX_ALBEDO = 6,
	HASTEX_PARTTEX_BLINN = 7,
	HASTEX_PARTTEX_PBR_3 = 8,
	HASTEX_PARTTEX_PBR_4 = 5,
};

enum Anti_Alising
{
	NO_AA = 0,
	MSAA4X = 1,
	MSAA8X = 2,
	MSAA16X = 3,
	MSAA32X = 4
};

enum Post_Process
{
	NO_PP = 0,
	INVERSE = 1,
	BLUR = 2,
	GRAY_SCALE = 3,
	EDGE = 4
};

enum Skybox_Type
{
	PURE_COLOR = 0,
	PICTURE = 1
};

// Structures
struct Settings
{
	unsigned int Width = 1280;
	unsigned int Height = 720;
	Core GraphicsCore = OPENGL;
	UI_Style UIStyle = DEFAULT_LIGHT;
	std::string FontStyle = "Open Sans";
	int FontSize = 16;
	float Gamma = 2.2f;
	Draw_Mode DrawMode = DEFAULT;
	float Resolution = 1.0f;
	Anti_Alising AA = NO_AA;
	Post_Process PP = NO_PP;
	bool ShowNormal = false;
};

struct Vertex
{
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Normal = glm::vec3(0.0f);
	glm::vec2 TexCoord = glm::vec2(0.0f);
	glm::vec4 MaterialIndex = glm::vec4(-1.0f);
	glm::vec3 NBDIndex = glm::vec3(-1.0f); // NBD stands for normal, bump and displacement
	glm::vec3 Tangent = glm::vec3(0.0f);
	glm::vec3 Bitangent = glm::vec3(0.0f);
};

struct ModelStatics
{
	unsigned int MeshCount = 0;
	unsigned int VertexCount = 0;
	unsigned int TriangleCount = 0;
	Render_Mode RenderMode = NOTEX_NOCOLOR;
};

struct SceneStatics
{
	unsigned int PointLight = 0;
	unsigned int SpotLight = 0;
	unsigned int DirectionalLight = 0;
};

// Statics and Consts
// Use these bools to open or close a window
namespace core
{
	// RenderBoy settings
	extern Settings SETTINGS;
	// buffer for loading file
	extern ImGui::FileBrowser FILE_BROWSER;
	extern Load_Type LOAD_TYPE;
	extern std::string FILEPATH_BUFFER;
	extern glm::vec3 NORMAL_COLOR;
	extern float NORMAL_MAGNITUDE;
	// Reload font flag
	extern bool RELOAD_FONT;
	// A pointer point to scene data
	extern void* SCENE_DATA;
	// used for combo widgets
	extern const char* currentModelInfo;
	extern const char* currentModelScene;
	extern const char* currentPointLight;
	extern const char* currentSpotLight;
	extern const char* currentDirLight;
	// bools to display windows
	extern bool IS_RENDERBOY_OPENED;
	extern bool IS_ABOUT_OPENED;
	extern bool IS_INFO_OPENED;
	extern bool IS_SETTINGS_OPENED;
	extern bool IS_WARNING_OPENED;
	extern bool IS_SCENE_OPENED;
	extern bool IS_MODELS_OPENED;
	extern bool IS_CAMERA_OPENED;
	extern bool IS_LIGHTS_OPENED;
	// message you want to show on warning popup
	extern std::string POPUP_MSG;
	// file types RenderBoy can read
	extern const std::vector<std::string> FILE_TYPE;
	extern const std::vector<std::string> FILE_TYPE_ASSIMP;
	extern const std::vector<std::string> FONT_STYLE;
}

// Use these statics to set ui style
namespace ImGui
{
	extern unsigned int COLOR_HIGHTLIGHT_STYLE;
	extern unsigned int COLOR_WARNING_SYTLE;
}

namespace core
{
	// Get current render resolution
	glm::vec2 GetRenderResolution();
	// Show popup message
	void ShowWarningMsg(std::string msg);
	// Do selected file format is supported
	bool CheckFileFormat(std::string fileType);
	bool CheckFileFormatAssimp(std::string fileType);
	// Get setting value from setting file
	std::string GetSettingValue(std::string setting);
	// Get scene value from scene file
	std::vector<std::string> GetSceneValue(std::string scene);
	// Get file info
	std::string GetFileDirectory(std::string filepath);
	std::string GetFileFormat(std::string filepath);
	std::string GetFileName(std::string filepath);
	std::string GetFileNameNoSuffix(std::string filepath);
	// Set UI style
	void SetUiStyle(UI_Style style);
	// Get Font Style
	std::string GetFontStylePath(std::string style);
	int GetFontStyleIndex(std::string style);
	// Get ImGui widgets width based on current font size
	float GetWidgetWidthCoefficient();
	// Get Rodrigue rotation matrix
	glm::mat4 GetRodrigue(glm::vec4 axis, float angle);
	// Get viewport matrix
	glm::mat4 GetViewPortMatrix(unsigned int width, unsigned int height);
	// Get attenuation values based on light's range.
	// If you want to modify these values, just modify this function
	// Values from https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
	glm::vec3 GetAttenuationValues(float range);
}

namespace ImGui
{
	// Center align widgets
	void CenterAlignWidget(std::string text);
	void CenterAlignWidget(float width);
	void CenterAlignWidget(std::string text, float width);
	// Make a colored label
	void LabelColored(std::string text, unsigned int color);
    void LabelHighlighted(std::string text);
	// Make a colored text
	void TextColored(std::string text, unsigned int color);
    void TextHighlighted(std::string text);
	// Convert uint to color
	unsigned int Color(unsigned int c);
	// Reset UI style
	void ResetStyleColors();
	// Set UI style to Spectrum. Source: https://github.com/adobe/imgui/blob/master/imgui_spectrum.h
	void StyleColorsSpectrum();
}