#include "Mesh.h"

Mesh::Mesh()
{
	m_AmbientValue = glm::vec3(0.0f);
	m_DiffuseValue = glm::vec3(0.0f);
	m_SpecularValue = glm::vec3(0.0f);
	m_EmissiveValue = glm::vec3(0.0f);
	m_ReflectiveValue = 0.0f;
	m_RefractionValue = 0.0f;
	m_TransparentValue = 0.0f;
	m_TransmissionValue = glm::vec3(0.0f);
	m_IllumValue = 0;
}

Mesh::~Mesh()
{
}

bool Mesh::HasTexture()
{
	bool hasTexture = false;
	hasTexture |= HasAlbedoTex();
	hasTexture |= HasSpecularTex();
	hasTexture |= HasMetallicTex();
	hasTexture |= HasRoughnessTex();
	hasTexture |= HasAoTex();
	hasTexture |= HasNormalTex();
	hasTexture |= HasHeightTex();
	return hasTexture;
}

bool Mesh::HasDiffuseTex()
{
	if (m_AlbedoTexFilePath != "")
	{
		return true;
	}
	return false;
}

bool Mesh::HasSpecularTex()
{
	if (m_SpecularTexFilePath != "")
	{
		return true;
	}
	return false;
}

bool Mesh::HasAlbedoTex()
{
	if (m_AlbedoTexFilePath != "")
	{
		return true;
	}
	return false;
}

bool Mesh::HasMetallicTex()
{
	if (m_MetallicTexFilePath != "")
	{
		return true;
	}
	return false;
}

bool Mesh::HasRoughnessTex()
{
	if (m_RoughnessTexFilePath != "")
	{
		return true;
	}
	return false;
}

bool Mesh::HasAoTex()
{
	if (m_AoTexFilePath != "")
	{
		return true;
	}
	return false;
}

bool Mesh::HasNormalTex()
{
	if (m_NormalTexFilePath != "")
	{
		return true;
	}
	return false;
}

bool Mesh::HasHeightTex()
{
	if (m_HeightTexFilePath != "")
	{
		return true;
	}
	return false;
}

bool Mesh::HasColorValue()
{
	if (m_DiffuseValue == glm::vec3(0.0f))
	{
		return false;
	}
	return true;
}
