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
#include "gl/opengl/GLDataBuffer.h"
#include "shader/opengl/GLShader.h"
#include "data/opengl/texture/GLTexture.h"
#include "data/opengl/texture/GLCubeMap.h"

struct GLPointLightData
{
	GLVertexArray VA;
	GLVertexBuffer VB;
	GLIndexBuffer IB;
	GLShader Shader;
	std::map<std::string, GLFrameBuffer> DepthMap;
};

struct GLSpotLightData
{
	GLVertexArray VA;
	GLVertexBuffer VB;
	GLIndexBuffer IB;
	GLShader Shader;
	std::map<std::string, GLFrameBuffer> DepthMap;
};

struct GLDirLightData
{
	GLVertexArray VA;
	GLVertexBuffer VB;
	GLIndexBuffer IB;
	GLShader Shader;
	std::map<std::string, std::vector<GLFrameBuffer>> DepthMap;
};

struct GLAreaLightData
{
	GLTexture LTC1;
	GLTexture LTC2;
	GLVertexArray RectangleVA;
	GLVertexBuffer RectangleVB;
	GLIndexBuffer RectangleIB;
	GLVertexArray SphereVA;
	GLVertexBuffer SphereVB;
	GLIndexBuffer SphereIB;
	GLVertexArray CylinderVA;
	GLVertexBuffer CylinderVB;
	GLIndexBuffer CylinderIB;
	GLVertexArray DiskVA;
	GLVertexBuffer DiskVB;
	GLIndexBuffer DiskIB;
	//std::map<std::string, std::vector<GLFrameBuffer>> DepthMap;
};

struct GLSkyboxData
{
	GLVertexArray VA;
	GLVertexBuffer VB;
	GLIndexBuffer IB;
	GLShader Shader;
	GLCubeMap Skybox;
};

struct GLVFXData
{
	std::vector<glm::vec3> SSAOSamples;
	GLTexture SSAONoiseTex;
	std::vector<unsigned int> ExpoHistogram;
	float ExpoAvg = 0.0f;
	GLDataBuffer ExpoHistoDB;
	GLDataBuffer ExpoAvgDB;
};

struct GLModelData
{
	ModelStatics Statics;
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
	std::vector<GLTexture> HeightTex;
	std::vector<glm::vec3> AmbientValue;
	std::vector<glm::vec3> DiffuseValue;
	std::vector<glm::vec3> SpecularValue;
	std::vector<glm::vec3> EmissiveValue;
	std::vector<float> ReflectiveValue;
	std::vector<float> TransparentValue;
	std::vector<float> RefractionValue;
	std::vector<glm::vec3> TransmissionValue;
};

class GLData
{
private:
	GLShader m_Shader; // Shader used for different draw mode (except default)
	GLSkyboxData m_SkyboxData;
	GLVFXData m_VFXData;
	GLPointLightData m_PointLightData;
	GLSpotLightData m_SpotLightData;
	GLDirLightData m_DirLightData;
	GLAreaLightData m_AreaLightData;
	std::vector<std::string> m_ModelList;
	std::map<std::string, GLModelData> m_ModelData;
	GLTexture m_CheckerMap;

	void AddPointLight(std::string name);
	void AddSpotLight(std::string name);
	void AddDirLight(std::string name);
	void AddAreaLight(std::string name);
	void DeletePointLight(std::string name);
	void DeleteSpotLight(std::string name);
	void DeleteDirLight(std::string name);
	void DeleteAreaLight(std::string name);
	void RenamePointLight(std::string oldName, std::string newName);
	void RenameSpotLight(std::string oldName, std::string newName);
	void RenameDirLight(std::string oldName, std::string newName);
	//void ReInitShader();

public:
	GLData();
	~GLData();

	void ReInitShader();

	bool Init();
	void Reset();
	// Change draw mode
	bool ChangeDrawMode();
	// Add, delete and rename model data
	void AddModel(std::string name, Model& model);
	bool DeleteModel(std::string name);
	bool RenameModel(std::string oldName, std::string newName);
	void AddLight(std::string name, Light_Type type);
	void DeleteLight(std::string name, Light_Type type);
	void RenameLight(std::string oldName, std::string newName, Light_Type type);
	bool LoadSkybox(std::vector<std::string> filepath);
	void SetShadowRes(std::string name, unsigned int width, unsigned int height, Light_Type type);
	// Get Data's members
	inline GLShader& GetShader()
	{
		return m_Shader;
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
	inline GLAreaLightData& GetAreaLightData()
	{
		return m_AreaLightData;
	};
	inline std::map<std::string, GLModelData>& GetModelData()
	{
		return m_ModelData;
	};
	inline GLVFXData& GetVFXData()
	{
		return m_VFXData;
	};
};