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
	inline std::vector<Vertex>& GetVertices()
	{
		return m_Vertices;
	};
	inline std::vector<unsigned int>& GetIndices()
	{
		return m_Indices;
	};
	inline std::vector<std::string>& GetDiffuseTexFilePaths()
	{
		return m_AlbedoTexFilePaths;
	};
	inline std::vector<std::string>& GetSpecularTexFilePaths()
	{
		return m_SpecularTexFilePaths;
	};
	inline std::vector<std::string>& GetAlbedoTexFilePaths()
	{
		return m_AlbedoTexFilePaths;
	};
	inline std::vector<std::string>& GetMetallicTexFilePaths()
	{
		return m_MetallicTexFilePaths;
	};
	inline std::vector<std::string>& GetRoughnessTexFilePaths()
	{
		return m_RoughnessTexFilePaths;
	};
	inline std::vector<std::string>& GetAoTexFilePaths()
	{
		return m_AoTexFilePaths;
	};
	inline std::vector<std::string>& GetNormalTexFilePaths()
	{
		return m_NormalTexFilePaths;
	};
	inline std::vector<std::string>& GetBumpTexFilePaths()
	{
		return m_BumpTexFilePaths;
	};
	inline std::vector<std::string>& GetDisplacementTexFilePaths()
	{
		return m_DisplacementTexFilePaths;
	};
	inline glm::vec3 GetAmbientValue()
	{
		return m_AmbientValue;
	};
	inline glm::vec3 GetDiffuseValue() const
	{
		return m_DiffuseValue;
	};
	inline glm::vec3 GetSpecularValue() const
	{
		return m_SpecularValue;
	};
	inline glm::vec3 GetEmissiveValue() const
	{
		return m_EmissiveValue;
	};
	inline float GetReflectionValue() const
	{
		return m_ReflectionValue;
	};
	inline float GetRefractionValue() const
	{
		return m_RefractionValue;
	};
	inline float GetTransparentValue() const
	{
		return m_TransparentValue;
	};
	inline float GetDissolveValue() const
	{
		return m_DissolveValue;
	};
	inline glm::vec3 GetTransmissionValue() const
	{
		return m_TransmissionValue;
	};
	inline unsigned int GetIllumValue() const
	{
		return m_IllumValue;
	};
};