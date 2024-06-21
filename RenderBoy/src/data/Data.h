/*
* Class:
*	Data
* Description:
*	This class contains all data renderer needs
*/

#pragma once

#include "opengl/GLData.h"

class Data
{
private:
	GLData m_DataGL;

public:
	Data();
	~Data();

	void Init();
	void Reset();
	// Change draw mode
	void ChangeDrawMode();
	// Add, delete and rename model
	void AddModel(std::string name, Model& model);
	void DeleteModel(std::string name);
	void RenameModel(std::string oldName, std::string newName);
	void AddLight(std::string name, Light_Type type);
	void DeleteLight(std::string name, Light_Type type);
	void RenameLight(std::string oldName, std::string newName, Light_Type type);
	bool LoadSkybox(std::vector<std::string> filepath);
	void SetShadowRes(std::string name, unsigned int width, unsigned int height, Light_Type type);
	// Get Data's members
	GLData& GetDataGL();
};