/*
* Class:
*	Scene
* Description:
*	This class contains all things inside the scene, including camera, models and lights
*/

#pragma once

#include "core/Core.h"
#include "data/Data.h"
#include "camera/Camera.h"
#include "model/Model.h"
#include "light/PointLight.h"
#include "light/SpotLight.h"
#include "light/DirectionalLight.h"
#include "light/AreaLight.h"

struct Skybox
{
	Skybox_Type Type = PURE_COLOR;
	glm::vec3 Color = glm::vec3(0.0f);
	std::vector<std::string> Filepath;
};

struct VFX_SSAO
{
	bool Status = false;
	int KernelSize = 64;
	float Radius = 0.5f;
	float Bias = 0.025f;
};

struct VFX_SSR
{
	bool Status = false;
	float MaxDistance = 500.0f;
	float Resolution = 0.5f;
	int Step = 10;
	float Thickness = 0.5f;
	int MaxLoop = 500;
};

struct VisualEffects
{
	VFX_SSAO SSAO;
	VFX_SSR SSR;
};

class Scene
{
private:
	std::string m_FilePath;
	std::string m_Name;
	Data m_Data;
	Camera m_Camera;
	Skybox m_Skybox;
	VisualEffects m_VFX;
	std::vector<std::string> m_ModelList;
	std::map<std::string, Model> m_Models;
	std::vector<std::string> m_PointLightList;
	std::map<std::string, PointLight> m_PointLights;
	std::vector<std::string> m_SpotLightList;
	std::map<std::string, SpotLight> m_SpotLights;
	std::vector<std::string> m_DirLightList;
	std::map<std::string, DirectionalLight> m_DirLights;
	std::vector<std::string> m_AreaLightList;
	std::map<std::string, AreaLight> m_AreaLights;

	// Draw windows
	void DrawSceneWindow();
	void DrawModelsWindow();
	void DrawCameraWindow();
	void DrawLightsWindow();

public:
	Scene();
	~Scene();

	bool Init();
	// Reset the current scene
	void Reset();
	bool Reset(std::string filepath);
	// Load scene from file
	bool LoadScene(std::string filepath);
	void SaveScene();
	// Add and delete model
	bool AddModel(std::string filepath);
	bool AddModel(std::string name, std::string filepath);
	bool DeleteModel(std::string name);
	// Add and delete light 
	bool AddLight(std::string name, Light_Type type);
	bool AddPointLight(std::string name);
	bool AddSpotLight(std::string name);
	bool AddDirectionalLight(std::string name);
	bool AddAreaLight(std::string name);
	bool DeleteLight(std::string name, Light_Type type);
	bool DeletePointLight(std::string name);
	bool DeleteSpotLight(std::string name);
	bool DeleteDirectionalLight(std::string name);
	bool DeleteAreaLight(std::string name);
	// Rename models and lights
	bool RenameModel(std::string oldName, std::string newName);
	bool RenameLight(std::string oldName, std::string newName, Light_Type type);
	bool RenamePointLight(std::string oldName, std::string newName);
	bool RenameSpotLight(std::string oldName, std::string newName);
	bool RenameDirectionalLight(std::string oldName, std::string newName);
	bool RenameAreaLight(std::string oldName, std::string newName);
	// Set scene's name
	void SetName(std::string name);
	// Get Scene's members
	inline std::string GetFilePath() const
	{
		return m_FilePath;
	};
	inline std::string GetName() const
	{
		return m_Name;
	};
	inline Data& GetData()
	{
		return m_Data;
	};
	inline Camera& GetCamera()
	{
		return m_Camera;
	};
	inline Skybox& GetSkybox()
	{
		return m_Skybox;
	};
	inline VisualEffects& GetVFX()
	{
		return m_VFX;
	};
	inline std::vector<std::string>& GetModelList()
	{
		return m_ModelList;
	};
	inline std::map<std::string, Model>& GetModels()
	{
		return m_Models;
	};
	inline std::vector<std::string>& GetPointLightList()
	{
		return m_PointLightList;
	};
	inline std::map<std::string, PointLight>& GetPointLights()
	{
		return m_PointLights;
	};
	inline std::vector<std::string>& GetSpotLightList()
	{
		return m_SpotLightList;
	};
	inline std::map<std::string, SpotLight>& GetSpotLights()
	{
		return m_SpotLights;
	};
	inline std::vector<std::string>& GetDirLightList()
	{
		return m_DirLightList;
	};
	inline std::map<std::string, DirectionalLight>& GetDirLights()
	{
		return m_DirLights;
	};
	inline std::vector<std::string>& GetAreaLightList()
	{
		return m_AreaLightList;
	};
	inline std::map<std::string, AreaLight>& GetAreaLights()
	{
		return m_AreaLights;
	};
	// Draw UI
	void DrawUI();
};