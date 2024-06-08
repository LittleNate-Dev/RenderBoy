/*
* Class:
*	Model
* Description:
*	This class abstract a model, each model contain various amount of meshes.
*	You can rotate, scale or translate a model, but you can do these to a single mesh.
*/

#pragma once

#include "core/Core.h"
#include "Mesh.h"

class Model
{
private:
	std::string m_FilePath;
	std::string m_Name;
	ModelStatics m_Statics;
	std::vector<Mesh> m_Meshes;
	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::vec3 m_EulerAngle;
	unsigned int m_Instance;
	unsigned int m_Current;
	bool m_SetInstance;
	std::vector<glm::vec3> m_InstancePosition;
	std::vector<glm::vec3> m_InstanceScale;
	std::vector<glm::vec3> m_InstanceEulerAngle;
	std::vector<glm::mat4> m_ModelMats;

	// After loading model, update model statics
	void UpdateStatics();
	// If filepath is correct, start to process model data
	bool LoadModelAssimp(std::string filepath);
	void AssimpProcessNode(aiNode* node, const aiScene* scene);
	Mesh AssimpProcessMesh(aiMesh* mesh, aiNode* node, const aiScene* scene);
	std::vector<std::string> AssimpLoadTexturePath(aiMaterial* mat, aiTextureType type);
	// After changing attributes, update matrix
	void UpdateModelMat(unsigned int current = 0);


public:
	Model();
	~Model();

	// Load model from file
	bool LoadModel(std::string name, std::string filepath);
	// Set Model's attribute
	void SetName(std::string name);
	void SetInstance(unsigned int instance);
	void SetCurrent(unsigned int current);
	void SetPosition(float posX, float posY, float posZ, unsigned int current = 0);
	void SetPosition(glm::vec3 position, unsigned int current = 0);
	void SetScale(float scaleX, float scaleY, float scaleZ, unsigned int current = 0);
	void SetScale(glm::vec3 scale, unsigned int current = 0);
	void SetPitch(float pitch, unsigned int current = 0);
	void SetYaw(float yaw, unsigned int current = 0);
	void SetRoll(float roll, unsigned int current = 0);
	void SetEulerAngle(float pitch, float yaw, float roll, unsigned int current = 0);
	void SetEulerAngle(glm::vec3 eulerAngle, unsigned int current = 0);
	// Get Model's members
	inline std::string GetFilePath() const
	{
		return m_FilePath;
	};
	inline std::string GetDirectory() const
	{
		return m_FilePath.substr(0, m_FilePath.find_last_of('/'));
	};
	inline std::string GetName() const
	{
		return m_Name;
	};
	inline ModelStatics GetStatics() const
	{
		return m_Statics;
	};
	inline std::vector<Mesh>& GetMeshes()
	{
		return m_Meshes;
	};
	glm::vec3 GetPosition(unsigned int current = 0);
	glm::vec3 GetScale(unsigned int current = 0);
	glm::vec3 GetEulerAngle(unsigned int current = 0);
	inline unsigned int GetInstance() const
	{
		return m_Instance;
	};
	inline unsigned int GetCurrent() const
	{
		return m_Current;
	};
	inline std::vector<glm::mat4>& GetModelMats()
	{
		return m_ModelMats;
	};
	glm::mat4 GetModelMat(unsigned int current = 0);
	glm::mat4 GetTranslateMat(unsigned int current = 0);
	glm::mat4 GetScaleMat(unsigned int current = 0);
	glm::mat4 GetRotateMat(unsigned int current = 0);
	// Draw Model's UI
	void DrawUI();
};