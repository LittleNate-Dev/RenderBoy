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
	std::string m_AlbedoTexFilePath;
	std::string m_SpecularTexFilePath;
	std::string m_MetallicTexFilePath;
	std::string m_RoughnessTexFilePath;
	std::string m_AoTexFilePath;
	std::string m_NormalTexFilePath;
	std::string m_HeightTexFilePath;
	glm::vec3 m_AmbientValue; // ka
	glm::vec3 m_DiffuseValue; // kd
	glm::vec3 m_SpecularValue; // ks
	glm::vec3 m_EmissiveValue; // ke
	float m_ReflectiveValue; // Ns
	float m_TransparentValue; // Tr
	float m_RefractionValue; // Ni
	glm::vec3 m_TransmissionValue; // Tf
	unsigned int m_IllumValue; // illum


public:
	Mesh();
	~Mesh();

	// Set Mesh's color value
	inline void SetAmbientValue(glm::vec3 value)
	{
		m_AmbientValue = value;
	};
	inline void SetDiffuseValue(glm::vec3 value)
	{
		m_DiffuseValue = value;
	};
	inline void SetSpecularValue(glm::vec3 value)
	{
		m_SpecularValue = value;
	};
	inline void SetEmissiveValue(glm::vec3 value)
	{
		m_EmissiveValue = value;
	};
	inline void SetReflectiveValue(float value)
	{
		m_ReflectiveValue = value;
	};
	inline void SetRefractionValue(float value)
	{
		m_RefractionValue = value;
	};
	inline void SetTransparentValue(float value)
	{
		m_TransparentValue = value;
	};
	inline void SetDissolveValue(float value)
	{
		m_TransparentValue = 1.0f - value;
	};
	inline void SetTransmissionValue(glm::vec3 value)
	{
		m_TransmissionValue = value;
	};
	inline void SetIllumValue(unsigned int value)
	{
		m_IllumValue = value;
	};

	// Does this mesh has ____ texture?
	bool HasTexture();
	bool HasDiffuseTex();
	bool HasSpecularTex();
	bool HasAlbedoTex();
	bool HasMetallicTex();
	bool HasRoughnessTex();
	bool HasAoTex();
	bool HasNormalTex();
	bool HasHeightTex();
	// Does this mesh has color value?
	bool HasColorValue();
	// Get Mesh's members
	inline std::vector<Vertex>& GetVertices()
	{
		return m_Vertices;
	};
	inline std::vector<unsigned int>& GetIndices()
	{
		return m_Indices;
	};
	inline std::string& GetDiffuseTexFilePath()
	{
		return m_AlbedoTexFilePath;
	};
	inline std::string& GetSpecularTexFilePath()
	{
		return m_SpecularTexFilePath;
	};
	inline std::string& GetAlbedoTexFilePath()
	{
		return m_AlbedoTexFilePath;
	};
	inline std::string& GetMetallicTexFilePath()
	{
		return m_MetallicTexFilePath;
	};
	inline std::string& GetRoughnessTexFilePath()
	{
		return m_RoughnessTexFilePath;
	};
	inline std::string& GetAoTexFilePath()
	{
		return m_AoTexFilePath;
	};
	inline std::string& GetNormalTexFilePath()
	{
		return m_NormalTexFilePath;
	};
	inline std::string& GetHeightTexFilePath()
	{
		return m_HeightTexFilePath;
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
	inline float GetReflectiveValue() const
	{
		return m_ReflectiveValue;
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
		return 1.0f - m_TransparentValue;
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