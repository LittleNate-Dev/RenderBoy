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
	// Add, delete and rename model
	void AddModel(std::string name, Model model);
	void DeleteModel(std::string name);
	void RenameModel(std::string oldName, std::string newName);
	bool LoadSkybox(std::vector<std::string> filepath);
	// Get Data's members
	GLData& GetDataGL();
};