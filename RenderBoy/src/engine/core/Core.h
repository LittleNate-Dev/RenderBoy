/*
* Description:
*	This header file contains every basic header file that is needed in this program, to keep things simple, you only need to include this header file instead of bunch of shits
*/

#pragma once

#include <iostream>
#include <stdlib.h>
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

#define APP_VERSION "ver 0.0.1"
#define SCENE_FILEPATH "data/saves/"
#define SETTING_FILEPATH "data/config/settings.config"
#define LOGGER_FILEPATH "data/logger/RenderBoy_Logger.txt"
#define SHADER_OPENGL "data/shaders/opengl/"
#define FONT_SIZE 1.2f
#define MAX_FOV 120.f
#define MIN_FOV 60.0f

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

enum UIStyle
{
	DEFAULT_LIGHT = 0,
	DEFAULT_DARK = 1,
	SPECTRUM = 2
};

enum LoadType
{
	NO_FILE = 0,
	RESET_SCENE = 1,
	ADD_MODEL = 2,
	LOAD_SKYBOX = 3
};

enum LightType
{
	POINT_LIGHT = 0,
	SPOT_LIGHT = 1,
	DIRECTIONAL_LIGHT = 2
};

enum DrawMode
{
	DEFAULT = 0,
	WIREFRAME = 1,
	POINTCLOUD = 2,
	UVSET = 3,
	DEPTH = 4
};

enum AntiAlising
{
	NO_AA = 0,
	MSAA4X = 1,
	MSAA8X = 2,
	MSAA16X = 3,
	MSAA32X = 4
};

enum PostProcess
{
	NO_PP = 0,
	INVERSE = 1,
	BLUR = 2,
	GRAY_SCALE = 3,
	EDGE = 4
};

// Structures
struct Settings
{
	unsigned int width = 1280;
	unsigned int height = 720;
	Core core = OPENGL;
	UIStyle uiStyle = DEFAULT_LIGHT;
	float gamma = 2.2f;
	DrawMode drawMode = DEFAULT;
	float resolution = 1.0f;
	AntiAlising aa = NO_AA;
	PostProcess pp = NO_PP;
	bool showNormal = false;
};

struct Vertex
{
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 normal = glm::vec3(0.0f);
	glm::vec2 texCoord = glm::vec2(0.0f);
	glm::vec4 texIndex = glm::vec4(-1.0f);
	glm::vec3 tangent = glm::vec3(0.0f);
	glm::vec3 bitangent = glm::vec3(0.0f);
};

struct ModelStatics
{
	unsigned int meshCount = 0;
	unsigned int vertexCount = 0;
	unsigned int triangleCount = 0;
};

struct SceneStatics
{
	unsigned int pointLight = 0;
	unsigned int spotLight = 0;
	unsigned int directionalLight = 0;
};

// Statics and Consts
// Use these bools to open or close a window
namespace rbcore
{
	// RenderBoy settings
	extern Settings SETTINGS;
	// buffer for loading file
	extern LoadType LOAD_TYPE;
	extern std::string FILEPATH_BUFFER;
	extern glm::vec3 NORMAL_COLOR;
	extern float NORMAL_MAGNITUDE;
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
	extern bool IS_MODELS_OPENED;
	extern bool IS_CAMERA_OPENED;
	extern bool IS_LIGHTS_OPENED;
	// message you want to show on warning popup
	extern std::string POPUP_MSG;
	// file types RenderBoy can read
	extern const std::vector<std::string> FILE_TYPE;
	extern const std::vector<std::string> FILE_TYPE_ASSIMP;
}
// Use these statics to set ui style
namespace ImGui
{
	extern unsigned int COLOR_HIGHTLIGHT_STYLE;
	extern unsigned int COLOR_WARNING_SYTLE;
}

namespace rbcore
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
	std::string GetFileType(std::string filepath);
	std::string GetFileName(std::string filepath);
	std::string GetFileNameNoSuffix(std::string filepath);
	// Set UI style
	void SetUiStyle(UIStyle style);
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