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

struct Skybox
{
	Skybox_Type Type = PURE_COLOR;
	glm::vec3 Color = glm::vec3(1.0f);
	std::vector<std::string> Filepath;
};

class Scene
{
private:
	std::string m_FilePath;
	std::string m_Name;
	Data m_Data;
	Camera m_Camera;
	Skybox m_Skybox;
	std::vector<std::string> m_ModelList;
	std::map<std::string, Model> m_Models;
	std::vector<std::string> m_PointLightList;
	std::map<std::string, PointLight> m_PointLights;
	std::vector<std::string> m_SpotLightList;
	std::map<std::string, SpotLight> m_SpotLights;
	std::vector<std::string> m_DirLightList;
	std::map<std::string, DirectionalLight> m_DirLights;

	// Draw windows
	void DrawSceneWindow();
	void DrawModelsWindow();
	void DrawCameraWindow();
	void DrawLightsWindow();

public:
	Scene();
	~Scene();

	void Init();
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
	bool DeleteLight(std::string name, Light_Type type);
	bool DeletePointLight(std::string name);
	bool DeleteSpotLight(std::string name);
	bool DeleteDirectionalLight(std::string name);
	// Rename models and lights
	bool RenameModel(std::string oldName, std::string newName);
	bool RenameLight(std::string oldName, std::string newName, Light_Type type);
	bool RenamePointLight(std::string oldName, std::string newName);
	bool RenameSpotLight(std::string oldName, std::string newName);
	bool RenameDirectionalLight(std::string oldName, std::string newName);
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
	inline std::vector<std::string>& GetDirectionalLightList()
	{
		return m_DirLightList;
	};
	inline std::map<std::string, DirectionalLight>& GetDirectionalLights()
	{
		return m_DirLights;
	};
	// Draw UI
	void DrawUI();
};