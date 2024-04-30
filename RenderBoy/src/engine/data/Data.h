/*
* Class:
*	Data
* Description:
*	This class contains all data renderer needs
*/

#pragma once

#include "opengl/DataGL.h"

class Data
{
private:
	DataGL m_DataGL;

public:
	Data();
	~Data();

	void Init();
	// Add, delete and rename model
	void AddModel(std::string name, Model model);
	void DeleteModel(std::string name);
	void RenameModel(std::string oldName, std::string newName);
	// Get Data's members
	DataGL& GetDataGL();
};