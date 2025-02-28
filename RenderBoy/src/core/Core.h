/*
* Description:
*	This header file contains every basic header file that is needed in this program,
*   to keep things simple, you only need to include this header file instead of bunch of shits
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
#include <random>
#include <Windows.h>

#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <Eigen/Dense>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imfilebrowser.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <spdlog/spdlog.h>

#include <freeimage/FreeImage.h>

#define APP_VERSION "ver 0.8.6"
#define GIT_REPO "https://github.com/LittleNate-Dev/RenderBoy"
#define SCENE_FILEPATH "saves/scene/"
#define SETTING_FILEPATH "data/config/settings.config"
#define LOGGER_FILEPATH "res/logger/RenderBoy_Logger.txt"
#define FONTS_FILEPATH "res/fonts/"
#define MODELS_FILEPATH "res/models/"
#define SCREENSHOT_FILEPATH "saves/screenshot/"
#define UV_MAP_FILEPATH "res/checkermap/CheckerMap.png"
#define SMAA_AREA_FILEPATH "data/shaders/tex/SMAA_Area.png"
#define SMAA_SEARCH_FILEPATH "data/shaders/tex/SMAA_Search.png"
#define LIGHTCUBE_SPHERE_MODEL "res/models/AreaLight_Sphere.obj"
#define LIGHTCUBE_CYLINDER_MODEL "res/models/AreaLight_Cylinder.obj"
#define LIGHTCUBE_DISK_MODEL "res/models/AreaLight_Disk.obj"
#define SHADER_OPENGL "data/shaders/opengl/"
#define SHADER_OPENGL_SCREEN "data/shaders/opengl/others/SCREEN.glsl"
#define SHADER_OPENGL_PP_NONE "data/shaders/opengl/post_process/NONE.glsl"
#define SHADER_OPENGL_PP_INVERSE "data/shaders/opengl/post_process/INVERSE.glsl"
#define SHADER_OPENGL_PP_BLUR "data/shaders/opengl/post_process/BLUR.glsl"
#define SHADER_OPENGL_PP_GRAYSCALE "data/shaders/opengl/post_process/GRAYSCALE.glsl"
#define SHADER_OPENGL_PP_EDGE "data/shaders/opengl/post_process/EDGE.glsl"
#define SHADER_OPENGL_OIT "data/shaders/opengl/others/OIT.glsl"
#define SHADER_OPENGL_RENDER_NOTEX "data/shaders/opengl/render_mode/NOTEX.glsl"
#define SHADER_OPENGL_RENDER_HASTEX_ALBEDO "data/shaders/opengl/render_mode/HASTEX_ALBEDO.glsl"
#define SHADER_OPENGL_RENDER_HASTEX_BLINN "data/shaders/opengl/render_mode/HASTEX_BLINN.glsl"
#define SHADER_OPENGL_RENDER_HASTEX_BLINN_NH "data/shaders/opengl/render_mode/HASTEX_BLINN_NH.glsl"
#define SHADER_OPENGL_RENDER_HASTEX_PBR_3 "data/shaders/opengl/render_mode/HASTEX_PBR_3.glsl"
#define SHADER_OPENGL_RENDER_HASTEX_PBR_4 "data/shaders/opengl/render_mode/HASTEX_PBR_4.glsl"
#define SHADER_OPENGL_RENDER_HASTEX_PBR_3_NH "data/shaders/opengl/render_mode/HASTEX_PBR_3_NH.glsl"
#define SHADER_OPENGL_RENDER_HASTEX_PBR_4_NH "data/shaders/opengl/render_mode/HASTEX_PBR_4_NH.glsl"
#define SHADER_OPENGL_GBUFFER "data/shaders/opengl/others/GBUFFER.glsl"
#define SHADER_OPENGL_GBUFFER_AREA "data/shaders/opengl/others/GBUFFER_AREA.glsl"
#define SHADER_OPENGL_BLANK "data/shaders/opengl/draw_mode/BLANK.glsl"
#define SHADER_OPENGL_WIREFRAME "data/shaders/opengl/draw_mode/WIREFRAME.glsl"
#define SHADER_OPENGL_DEPTH "data/shaders/opengl/draw_mode/DEPTH.glsl"
#define SHADER_OPENGL_UVSET "data/shaders/opengl/draw_mode/UVSET.glsl"
#define SHADER_OPENGL_POINTCLOUD "data/shaders/opengl/draw_mode/POINTCLOUD.glsl"
#define SHADER_OPENGL_NORMAL_DM "data/shaders/opengl/draw_mode/NORMAL_DM.glsl"
#define SHADER_OPENGL_SKYBOX "data/shaders/opengl/others/SKYBOX.glsl"
#define SHADER_OPENGL_NORMAL "data/shaders/opengl/others/NORMAL.glsl"
#define SHADER_OPENGL_LIGHTCUBE "data/shaders/opengl/others/LIGHTCUBE.glsl"
#define SHADER_OPENGL_LIGHTCUBE_AREA "data/shaders/opengl/others/LIGHTCUBE_AREA.glsl"
#define SHADER_OPENGL_AA_FXAA "data/shaders/opengl/aa/FXAA.glsl"
#define SHADER_OPENGL_AA_SMAA_1X_EDGE "data/shaders/opengl/aa/smaa/EDGE.glsl"
#define SHADER_OPENGL_AA_SMAA_1X_WEIGHT "data/shaders/opengl/aa/smaa/WEIGHT.glsl"
#define SHADER_OPENGL_AA_SMAA_1X_BLEND "data/shaders/opengl/aa/smaa/BLEND.glsl"
#define SHADER_OPENGL_BLOOM_DOWNSAMPLE "data/shaders/opengl/vfx/bloom/DOWNSAMPLE.glsl"
#define SHADER_OPENGL_BLOOM_UPSAMPLE "data/shaders/opengl/vfx/bloom/UPSAMPLE.glsl"
#define SHADER_OPENGL_BLOOM_BLEND "data/shaders/opengl/vfx/bloom/BLEND.glsl"
#define SHADER_OPENGL_DOF_COC "data/shaders/opengl/vfx/dof/COC.glsl"
#define SHADER_OPENGL_DOF_DOWNSAMPLE "data/shaders/opengl/vfx/dof/DOWNSAMPLE.glsl"
#define SHADER_OPENGL_DOF_BOKEH "data/shaders/opengl/vfx/dof/BOKEH.glsl"
#define SHADER_OPENGL_DOF_BLEND "data/shaders/opengl/vfx/dof/BLEND.glsl"
#define SHADER_OPENGL_SSAO_GEN "data/shaders/opengl/vfx/ssao/GEN.glsl"
#define SHADER_OPENGL_SSAO_BLUR "data/shaders/opengl/vfx/ssao/BLUR.glsl"
#define SHADER_OPENGL_SHADOW_POINT "data/shaders/opengl/shadow/POINTLIGHT.glsl"
#define SHADER_OPENGL_SHADOW_SPOT "data/shaders/opengl/shadow/SPOTLIGHT.glsl"
#define SHADER_OPENGL_SHADOW_DIR "data/shaders/opengl/shadow/DIRLIGHT.glsl"
#define SHADER_OPENGL_UTIL_GAUSSIAN_BLUR "data/shaders/opengl/util/GAUSSIAN_BLUR.glsl"
#define SHADER_OPENGL_EXPOSURE_HISTOGRAM "data/shaders/opengl/vfx/exposure/HISTOGRAM.glsl"
#define SHADER_OPENGL_EXPOSURE_AVERAGE "data/shaders/opengl/vfx/exposure/AVERAGE.glsl"
#define DRAW_MODE_COUNT 7
#define POST_PROCESS_COUNT 5
#define SETTING_MAX_FOV 200
#define SETTING_MIN_FOV 40
#define SETTING_RES_MIN 20
#define SETTING_RES_MAX 200
#define SETTING_MOUSE_MIN_SEN 0.1f
#define SETTING_MOUSE_MAX_SEN 2.0f
#define SETTING_VFX_DOF_MIN_FLENGTH 0.1f
#define SETTING_VFX_DOF_MAX_FLENGTH 10.0f

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

enum Shader_Type
{
	NONE = -1, 
	VERTEX = 0, 
	GEOMETRY = 1, 
	FRAGMENT = 2,
	COMPUTE = 3
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
	DIRECTIONAL_LIGHT = 2,
	AREA_LIGHT = 3
};

enum Draw_Mode
{
	DEFAULT = 0,
	BLANK = 1,
	WIREFRAME = 2,
	POINTCLOUD = 3,
	UVSET = 4,
	DEPTH = 5,
	NORMAL_DM = 6
};

enum Render_Mode
{
	NOTEX = 0,
	HASTEX_ALBEDO = 1,
	HASTEX_BLINN = 2,
	HASTEX_PBR_3 = 3,
	HASTEX_PBR_4 = 4,
	HASTEX_BLINN_NH = 5,
	HASTEX_PBR_3_NH = 6,
	HASTEX_PBR_4_NH = 7
};

enum Anti_Alising
{
	NO_AA = 0,
	FXAA = 1,
	SMAA_1X = 2
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

enum Tonemap_Curve
{
	REINHARD = 0,
	REINHARD2 = 1,
	ACES = 2,
	UCHIMURA = 3,
	LOTTES = 4
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
	Tonemap_Curve TonemapCurve = REINHARD;
	bool ShowNormal = false;
	glm::vec3 NormalColor = glm::vec3(1.0f);
	float NormalMagnitude = 1.0f;
	float Sensitivity = 1.0f;
	bool EnableController = false;
	glm::vec2 DeadZone = glm::vec2(0.05f, 0.05f);
	bool FullScreen = true;
};

struct Vertex
{
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec2 TexCoord = glm::vec2(0.0f);
	glm::vec3 Normal = glm::vec3(0.0f);
	glm::vec3 Tangent = glm::vec3(0.0f);
	glm::vec3 Bitangent = glm::vec3(0.0f);
	glm::vec4 TexIndex = glm::vec4(-1.0f);
	glm::vec3 ColorIndex = glm::vec3(-1.0f);
	glm::vec4 AttributeIndex = glm::vec4(-1.0f); // Ns, Ni, Tr, Tf
	glm::vec2 NHIndex = glm::vec2(-1.0f); // NBD stands for normal, bump and displacement
};

struct ModelStatics
{
	unsigned int MeshCount = 0;
	unsigned int VertexCount = 0;
	unsigned int TriangleCount = 0;
	Render_Mode RenderMode = NOTEX;
	unsigned int AlbedoTexCount = 0;
	unsigned int SpecularTexCount = 0;
	unsigned int MetallicTexCount = 0;
	unsigned int RoughnessTexCount = 0;
	unsigned int AoTexCount = 0;
	unsigned int NormalTexCount = 0;
	unsigned int HeightTexCount = 0;
	unsigned int AmbientValueCount = 0;
	unsigned int DiffuseValueCount = 0;
	unsigned int SpecularValueCount = 0;
	unsigned int EmissiveValueCount = 0;
	unsigned int ReflectiveCount = 0;
	unsigned int TransparentCount = 0;
};

struct SceneStatics
{
	unsigned int PointLight = 0;
	unsigned int SpotLight = 0;
	unsigned int DirectionalLight = 0;
	unsigned int AreaLight = 0;
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
	// Reload font flag
	extern bool RELOAD_FONT;
	// A pointer point to scene data
	extern void* SCENE_DATA;
	extern SceneStatics SCENE_STATICS;
	extern glm::vec2 RENDER_RES;
	// system time
	extern float CURRENT_TIME;
	extern float LAST_TIME;
	extern float TIME_DELTA;
	// used for combo widgets
	extern const char* currentModelScene;
	extern const char* currentPointLight;
	extern const char* currentSpotLight;
	extern const char* currentDirLight;
	extern const char* currentAreaLight;
	// bools to display windows
	extern bool IS_RENDERBOY_OPENED;
	extern bool IS_UI_OPENED;
	extern bool IS_ABOUT_OPENED;
	extern bool IS_PERF_OPENED;
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
	void UpdateRenderRes();
	glm::vec2 GetRenderRes();
	// Show popup message
	void ShowWarningMsg(std::string msg);
	// Do selected file format is supported
	bool CheckFileFormat(std::string fileType);
	bool CheckFileFormatAssimp(std::string fileType);
	// Get scene value from scene file
	std::vector<std::string> GetFileValue(std::string line);
	// Get file info
	std::string GetFileDirectory(std::string filepath);
	std::string GetFileFormat(std::string filepath);
	std::string GetFileName(std::string filepath);
	std::string GetFileNameNoSuffix(std::string filepath);
	// Replace all \ in a filepath with /
	std::string ReplaceBackwardSlash(std::string filepath);
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