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
#include "data/opengl/texture/GLTexture.h"
#include "data/opengl/texture/GLCubeMap.h"

struct GLLightCubeData
{
	GLVertexArray VA;
	GLVertexBuffer VB;
	GLIndexBuffer IB;
};

struct GLModelData
{
	GLVertexArray VA;
	GLVertexBuffer VB;
	GLVertexBuffer InstanceVB;
	GLIndexBuffer IB;
	std::vector<GLTexture> AlbedoTex;
	std::vector<GLTexture> SpecularTex;
	std::vector<GLTexture> MetallicTex;
	std::vector<GLTexture> RoughnessTex;
	std::vector<GLTexture> AoTex;
	std::vector<GLTexture> NormalTex;
	std::vector<GLTexture> BumpTex;
	std::vector<GLTexture> DisplacementTex;
	std::vector<glm::vec3> AmbientValue;
	std::vector<glm::vec3> DiffuseValue;
	std::vector<glm::vec3> SpecularValue;
	std::vector<glm::vec3> EmissiveValue;
};

struct GLSkyboxData
{
	GLVertexArray VA;
	GLVertexBuffer VB;
	GLIndexBuffer IB;
	GLCubeMap Skybox;
};

class GLData
{
private:
	GLSkyboxData m_SkyboxData;
	GLLightCubeData m_PointLightCube;
	GLLightCubeData m_SpotLightCube;
	GLLightCubeData m_DirLightCube;
	std::map<std::string, GLModelData> m_ModelData;


public:
	GLData();
	~GLData();

	void Init();
	void Reset();
	// Add, delete and rename model data
	void AddModel(std::string name, Model model);
	bool DeleteModel(std::string name);
	bool RenameModel(std::string oldName, std::string newName);
	bool LoadSkybox(std::vector<std::string> filepath);
	// Get Data's members
	inline GLSkyboxData& GetSkybox()
	{
		return m_SkyboxData;
	};
	GLLightCubeData& GetLightCube(Light_Type type);
	inline std::map<std::string, GLModelData>& GetModelData()
	{
		return m_ModelData;
	};
};