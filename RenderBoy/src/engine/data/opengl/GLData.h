/*
* Class:
*	GLData
* Description:
*	This class contains all data needed when rendering with OpenGL
*/

#pragma once

#include "core/Core.h"
#include "scene/model/Model.h"
#include "gl/opengl/GLVertexArray.h"
#include "gl/opengl/GLVertexBuffer.h"
#include "gl/opengl/GLVertexBufferLayout.h"
#include "gl/opengl/GLIndexBuffer.h"

struct LightCubeDataGL
{
	GLVertexArray VA;
	GLVertexBuffer VB;
	GLIndexBuffer IB;
};

struct ModelDataGL
{
	GLVertexArray VA;
	GLVertexBuffer VB;
	GLVertexBuffer InstanceVB;
	GLIndexBuffer IB;
};

class GLData
{
private:
	LightCubeDataGL m_PointLightCube;
	LightCubeDataGL m_SpotLightCube;
	LightCubeDataGL m_DirLightCube;
	std::map<std::string, ModelDataGL> m_ModelData;

public:
	GLData();
	~GLData();

	void Init();
	void Reset();
	// Add, delete and rename model data
	void AddModel(std::string name, Model model);
	bool DeleteModel(std::string name);
	bool RenameModel(std::string oldName, std::string newName);
	// Get Data's members
	LightCubeDataGL& GetLightCube(Light_Type type);
	inline std::map<std::string, ModelDataGL>& GetModelData()
	{
		return m_ModelData;
	};
};