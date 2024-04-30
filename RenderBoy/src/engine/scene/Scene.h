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

class Scene
{
private:
	std::string m_FilePath;
	std::string m_Name;
	Data m_Data;
	Camera m_Camera;
	std::vector<std::string> m_ModelList;
	std::map<std::string, Model> m_Models;
	std::vector<std::string> m_PointLightList;
	std::map<std::string, PointLight> m_PointLights;
	std::vector<std::string> m_SpotLightList;
	std::map<std::string, SpotLight> m_SpotLights;
	std::vector<std::string> m_DirLightList;
	std::map<std::string, DirectionalLight> m_DirLights;

	// Draw windows
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
	bool AddLight(std::string name, LightType type);
	bool AddPointLight(std::string name);
	bool AddSpotLight(std::string name);
	bool AddDirectionalLight(std::string name);
	bool DeleteLight(std::string name, LightType type);
	bool DeletePointLight(std::string name);
	bool DeleteSpotLight(std::string name);
	bool DeleteDirectionalLight(std::string name);
	// Rename models and lights
	bool RenameModel(std::string oldName, std::string newName);
	bool RenameLight(std::string oldName, std::string newName, LightType type);
	bool RenamePointLight(std::string oldName, std::string newName);
	bool RenameSpotLight(std::string oldName, std::string newName);
	bool RenameDirectionalLight(std::string oldName, std::string newName);
	// Set scene's name
	void SetName(std::string name);
	// Get Scene's members
	std::string GetFilePath();
	std::string GetName();
	Data& GetData();
	Camera& GetCamera();
	std::vector<std::string>& GetModelList();
	std::map<std::string, Model>& GetModels();
	std::vector<std::string>& GetPointLightList();
	std::map<std::string, PointLight>& GetPointLights();
	std::vector<std::string>& GetSpotLightList();
	std::map<std::string, SpotLight>& GetSpotLights();
	std::vector<std::string>& GetDirectionalLightList();
	std::map<std::string, DirectionalLight>& GetDirectionalLights();
	// Draw UI
	void DrawUI();
};