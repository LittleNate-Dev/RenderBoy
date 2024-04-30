/*
* Class:
*	Mesh
* Description:
*	This class abstract a mesh, each mesh is a part of the model, and contain various
*	amount of vertices, and it holds the filepath of textures.
*/

#pragma once

#include "core/Core.h"

class Mesh
{
private:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<std::string> m_AlbedoTexFilePaths;
	std::vector<std::string> m_SpecularTexFilePaths;
	std::vector<std::string> m_MetallicTexFilePaths;
	std::vector<std::string> m_RoughnessTexFilePaths;
	std::vector<std::string> m_AoTexFilePaths;
	std::vector<std::string> m_NormalTexFilePaths;
	std::vector<std::string> m_BumpTexFilePaths;
	std::vector<std::string> m_DisplacementTexFilePaths;
	glm::vec3 m_AmbientValue; // ka
	glm::vec3 m_DiffuseValue; // kd
	glm::vec3 m_SpecularValue; // ks
	glm::vec3 m_EmissiveValue; // ke
	float m_ReflectionValue; // Ns
	float m_RefractionValue; // Ni
	float m_TransparentValue; // Tr
	float m_DissolveValue; // d
	glm::vec3 m_TransmissionValue; // Tf
	unsigned int m_IllumValue; // illum


public:
	Mesh();
	~Mesh();

	// Set Mesh's color value
	void SetAmbientValue(glm::vec3 value);
	void SetDiffuseValue(glm::vec3 value);
	void SetSpecularValue(glm::vec3 value);
	void SetEmissiveValue(glm::vec3 value);
	void SetReflectionValue(float value);
	void SetRefractionValue(float value);
	void SetTransparentValue(float value);
	void SetDissolveValue(float value);
	void SetTransmissionValue(glm::vec3 value);
	void SetIllumValue(unsigned int value);

	// Do this mesh has ____ texture?
	bool HasTexture();
	bool HasDiffuseTex();
	bool HasSpecularTex();
	bool HasAlbedoTex();
	bool HasMetallicTex();
	bool HasRoughnessTex();
	bool HasAoTex();
	bool HasNormalTex();
	bool HasBumpTex();
	bool HasDisplacementTex();
	// Get Mesh's members
	std::vector<Vertex>& GetVertices();
	std::vector<unsigned int>& GetIndices();
	std::vector<std::string>& GetDiffuseTexFilePaths();
	std::vector<std::string>& GetSpecularTexFilePaths();
	std::vector<std::string>& GetAlbedoTexFilePaths();
	std::vector<std::string>& GetMetallicTexFilePaths();
	std::vector<std::string>& GetRoughnessTexFilePaths();
	std::vector<std::string>& GetAoTexFilePaths();
	std::vector<std::string>& GetNormalTexFilePaths();
	std::vector<std::string>& GetBumpTexFilePaths();
	std::vector<std::string>& GetDisplacementTexFilePaths();
	glm::vec3 GetAmbientValue();
	glm::vec3 GetDiffuseValue();
	glm::vec3 GetSpecularValue();
	glm::vec3 GetEmissiveValue();
	float GetReflectionValue();
	float GetRefractionValue();
	float GetTransparentValue();
	float GetDissolveValue();
	glm::vec3 GetTransmissionValue();
	unsigned int GetIllumValue();
};