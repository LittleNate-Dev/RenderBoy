/*
* Class:
*	GLData
* Description:
*	This class contains all data needed when rendering with OpenGL
*/

#pragma once

#include "core/Core.h"
#include "scene/model/Model.h"
#include "gl/opengl/GLFrameBuffer.h"
#include "gl/opengl/GLVertexArray.h"
#include "gl/opengl/GLVertexBuffer.h"
#include "gl/opengl/GLVertexBufferLayout.h"
#include "gl/opengl/GLIndexBuffer.h"
#include "shader/opengl/GLShader.h"
#include "data/opengl/texture/GLTexture.h"
#include "data/opengl/texture/GLCubeMap.h"

struct GLPointLightData
{
	GLVertexArray VA;
	GLVertexBuffer VB;
	GLIndexBuffer IB;
	std::map<std::string, GLFrameBuffer> DepthMap;
};

struct GLSpotLightData
{
	GLVertexArray VA;
	GLVertexBuffer VB;
	GLIndexBuffer IB;
	std::map<std::string, GLFrameBuffer> DepthMap;
};

struct GLDirLightData
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
	GLShader Shader;
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
	std::vector<float> ReflectiveValue;
	std::vector<float> TransparentValue;
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
	GLShader m_BlankShader;
	GLSkyboxData m_SkyboxData;
	GLPointLightData m_PointLightData;
	GLSpotLightData m_SpotLightData;
	GLDirLightData m_DirLightData;
	std::map<std::string, GLModelData> m_ModelData;

	void AddPointLight(std::string name);
	void AddSpotLight(std::string name);
	void AddDirLight(std::string name);
	void DeletePointLight(std::string name);
	void DeleteSpotLight(std::string name);
	void DeleteDirLight(std::string name);
	void RenamePointLight(std::string oldName, std::string newName);
	void RenameSpotLight(std::string oldName, std::string newName);
	void RenameDirLight(std::string oldName, std::string newName);

public:
	GLData();
	~GLData();

	void Init();
	void Reset();
	// Add, delete and rename model data
	void AddModel(std::string name, Model model);
	bool DeleteModel(std::string name);
	bool RenameModel(std::string oldName, std::string newName);
	void AddLight(std::string name, Light_Type type);
	void DeleteLight(std::string name, Light_Type type);
	void RenameLight(std::string oldName, std::string newName, Light_Type type);
	bool LoadSkybox(std::vector<std::string> filepath);
	// Get Data's members
	inline GLShader& GetBlankShader()
	{
		return m_BlankShader;
	};
	inline GLSkyboxData& GetSkybox()
	{
		return m_SkyboxData;
	};
	inline GLPointLightData& GetPointLightData()
	{
		return m_PointLightData;
	};
	inline GLSpotLightData& GetSpotLightData()
	{
		return m_SpotLightData;
	};
	inline GLDirLightData& GetDirLightData()
	{
		return m_DirLightData;
	};
	inline std::map<std::string, GLModelData>& GetModelData()
	{
		return m_ModelData;
	};
};