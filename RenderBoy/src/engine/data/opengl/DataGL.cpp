#include "DataGL.h"

DataGL::DataGL()
{
}

DataGL::~DataGL()
{
}

void DataGL::Init()
{
	// Initialize point light's cube
	{
		float position[] = {
			0.0f,  1.0f,  0.0f, // 0
			1.0f,  0.0f, -1.0f, // 1
			1.0f,  0.0f,  1.0f, // 2
		   -1.0f,  0.0f,  1.0f, // 3
		   -1.0f,  0.0f, -1.0f, // 4
			0.0f, -1.0f,  0.0f  // 5
		};
		unsigned int indices[] = {
			0, 1, 2,
			0, 2, 3,
			0, 3, 4,
			0, 4, 1,
			5, 1, 2,
			5, 2, 3,
			5, 3, 4,
			5, 4, 1
		};
		m_PointLightCube.va.GenVertexArray();
		m_PointLightCube.vb.GenVertexBuffer(position, sizeof(position));
		m_PointLightCube.ib.GenIndexBuffer(indices, 24);
		GLVertexBufferLayout layout;
		layout.Push<float>(3);
		m_PointLightCube.va.AddBuffer(m_PointLightCube.vb, layout);
	}
	// Initialize spot light's cube
	{
		float position[] = {
			0.0f,  0.0f,  0.0f, // 0
			1.0f,  1.0f,  1.0f, // 1
			1.0f, -1.0f,  1.0f, // 2
		   -1.0f, -1.0f,  1.0f, // 3
	       -1.0f,  1.0f,  1.0f  // 4
		};
		unsigned int indices[] = {
			0, 1, 2,
			0, 2, 3,
			0, 3, 4,
			0, 4, 1,
			4, 1, 2,
			4, 2, 3
		};
		m_SpotLightCube.va.GenVertexArray();
		m_SpotLightCube.vb.GenVertexBuffer(position, sizeof(position));
		m_SpotLightCube.ib.GenIndexBuffer(indices, 18);
		GLVertexBufferLayout layout;
		layout.Push<float>(3);
		m_SpotLightCube.va.AddBuffer(m_SpotLightCube.vb, layout);
	}
	// Initialize directional light's cube
	{
		float position[] = {
			0.1f,  0.1f,  0.0f, // 0
			0.1f, -0.1f,  0.0f, // 1
		   -0.1f, -0.1f,  0.0f, // 2
		   -0.1f,  0.1f,  0.0f, // 3
			0.1f,  0.1f,  8.0f, // 4
			0.1f, -0.1f,  8.0f, // 5
		   -0.1f, -0.1f,  8.0f, // 6
		   -0.1f,  0.1f,  8.0f, // 7
		};
		unsigned int indices[] = {
			0, 1, 2,
			0, 2, 3,
			4, 5, 6,
			4, 6, 7,
			7, 6, 2,
			7, 2, 3,
			6, 5, 1,
			6, 1, 2,
			5, 4, 0,
			5, 0, 1,
			4, 3, 0,
			4, 7, 3
		};
		m_DirLightCube.va.GenVertexArray();
		m_DirLightCube.vb.GenVertexBuffer(position, sizeof(position));
		m_DirLightCube.ib.GenIndexBuffer(indices, 36);
		GLVertexBufferLayout layout;
		layout.Push<float>(3);
		m_DirLightCube.va.AddBuffer(m_DirLightCube.vb, layout);
	}
}

void DataGL::AddModel(std::string name, Model model)
{
	ModelDataGL modelData;
	m_ModelData.insert(std::pair<std::string, ModelDataGL>(name, modelData));
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < model.GetMeshes().size(); i++)
	{
		unsigned int indexOffset = (unsigned int)vertices.size();
		for (unsigned int j = 0; j < model.GetMeshes()[i].GetIndices().size(); j++)
		{
			indices.push_back(model.GetMeshes()[i].GetIndices()[j] + indexOffset);
		}
		vertices.insert(vertices.end(), model.GetMeshes()[i].GetVertices().begin(), model.GetMeshes()[i].GetVertices().end());
	}
	m_ModelData[name].va.GenVertexArray();
	m_ModelData[name].vb.GenVertexBuffer(&vertices[0], (unsigned int)vertices.size() * sizeof(Vertex));
	m_ModelData[name].ib.GenIndexBuffer(&indices[0], (unsigned int)indices.size());
	GLVertexBufferLayout layout;
	//vertex positions
	layout.Push<float>(3);
	//vertex normals
	layout.Push<float>(3);
	//vertex texture coords
	layout.Push<float>(2);
	//vertex texture index
	layout.Push<float>(4);
	//vertex tangent
	layout.Push<float>(3);
	//vertex bitangent
	layout.Push<float>(3);
	m_ModelData[name].va.AddBuffer(m_ModelData[name].vb, layout);
	// Add model matrix as vertex attribute
	m_ModelData[name].instanceVb.GenVertexBuffer(&model.GetModelMats()[0], (unsigned int)model.GetModelMats().size() * sizeof(glm::mat4));
	GLVertexBufferLayout instanceLayout;
	instanceLayout.Push<float>(4);
	instanceLayout.Push<float>(4);
	instanceLayout.Push<float>(4);
	instanceLayout.Push<float>(4);
	m_ModelData[name].va.AddBuffer(m_ModelData[name].instanceVb, instanceLayout, 1);

}

bool DataGL::DeleteModel(std::string name)
{
	if (m_ModelData.find(name) != m_ModelData.end())
	{
		m_ModelData.erase(name);
		return true;
	}
	return false;
}

bool DataGL::RenameModel(std::string oldName, std::string newName)
{
	if (m_ModelData.find(oldName) != m_ModelData.end())
	{
		auto model = m_ModelData.extract(oldName);
		model.key() = newName;
		m_ModelData.insert(std::move(model));
		return true;
	}
	return false;
}

LightCubeDataGL& DataGL::GetLightCube(LightType type)
{
	switch (type)
	{
	case POINT_LIGHT:
		return m_PointLightCube;
	case SPOT_LIGHT:
		return m_SpotLightCube;
	case DIRECTIONAL_LIGHT:
		return m_DirLightCube;
	}
}

std::map<std::string, ModelDataGL>& DataGL::GetModelData()
{
	return m_ModelData;
}
