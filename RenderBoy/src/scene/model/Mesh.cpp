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

void Mesh::SetAmbientValue(glm::vec3 value)
{
	m_AmbientValue = value;
}

void Mesh::SetDiffuseValue(glm::vec3 value)
{
	m_DiffuseValue = value;
}

void Mesh::SetSpecularValue(glm::vec3 value)
{
	m_SpecularValue = value;
}

void Mesh::SetEmissiveValue(glm::vec3 value)
{
	m_EmissiveValue = value;
}

void Mesh::SetReflectiveValue(float value)
{
	m_ReflectiveValue = value;
}

void Mesh::SetRefractionValue(float value)
{
	m_RefractionValue = value;
}

void Mesh::SetTransparentValue(float value)
{
	m_TransparentValue = value;
}

void Mesh::SetDissolveValue(float value)
{
	m_TransparentValue = 1.0f - value;
}

void Mesh::SetTransmissionValue(glm::vec3 value)
{
	m_TransmissionValue = value;
}

void Mesh::SetIllumValue(unsigned int value)
{
	m_IllumValue = value;
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
	hasTexture |= HasBumpTex();
	hasTexture |= HasDisplacementTex();
	return hasTexture;
}

bool Mesh::HasDiffuseTex()
{
	if (m_AlbedoTexFilePaths.size())
	{
		return true;
	}
	return false;
}

bool Mesh::HasSpecularTex()
{
	if (m_SpecularTexFilePaths.size())
	{
		return true;
	}
	return false;
}

bool Mesh::HasAlbedoTex()
{
	if (m_AlbedoTexFilePaths.size())
	{
		return true;
	}
	return false;
}

bool Mesh::HasMetallicTex()
{
	if (m_MetallicTexFilePaths.size())
	{
		return true;
	}
	return false;
}

bool Mesh::HasRoughnessTex()
{
	if (m_RoughnessTexFilePaths.size())
	{
		return true;
	}
	return false;
}

bool Mesh::HasAoTex()
{
	if (m_AoTexFilePaths.size())
	{
		return true;
	}
	return false;
}

bool Mesh::HasNormalTex()
{
	if (m_NormalTexFilePaths.size())
	{
		return true;
	}
	return false;
}

bool Mesh::HasBumpTex()
{
	if (m_BumpTexFilePaths.size())
	{
		return true;
	}
	return false;
}

bool Mesh::HasDisplacementTex()
{
	if (m_DisplacementTexFilePaths.size())
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
