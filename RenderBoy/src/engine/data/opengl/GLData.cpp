#include "GLData.h"

GLData::GLData()
{
}

GLData::~GLData()
{
}

void GLData::Init()
{
	// Initialize VAO to draw skybox
	{
		float position[] = {
			// positions          
			-1.0f,  1.0f, -1.0f, //0
			-1.0f, -1.0f, -1.0f, //1
			 1.0f, -1.0f, -1.0f, //2
			 1.0f,  1.0f, -1.0f, //3
			-1.0f, -1.0f,  1.0f, //4
			-1.0f,  1.0f,  1.0f, //5
			 1.0f, -1.0f,  1.0f, //6
			 1.0f,  1.0f,  1.0f  //7
		};
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0,
			4, 1, 0,
			0, 5, 4,
			2, 6, 7,
			7 ,3, 2,
			4, 5, 7,
			7, 6, 4,
			0, 3, 7,
			7, 5, 0,
			1, 4, 2,
			2, 4, 6
		};
		m_SkyboxData.VA.GenVertexArray();
		m_SkyboxData.VB.GenVertexBuffer(position, sizeof(position));
		m_SkyboxData.IB.GenIndexBuffer(indices, 36);
		GLVertexBufferLayout layout;
		layout.Push<float>(3);
		m_SkyboxData.VA.AddBuffer(m_SkyboxData.VB, layout);
	}
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
		m_PointLightData.VA.GenVertexArray();
		m_PointLightData.VB.GenVertexBuffer(position, sizeof(position));
		m_PointLightData.IB.GenIndexBuffer(indices, 24);
		GLVertexBufferLayout layout;
		layout.Push<float>(3);
		m_PointLightData.VA.AddBuffer(m_PointLightData.VB, layout);
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
		m_SpotLightData.VA.GenVertexArray();
		m_SpotLightData.VB.GenVertexBuffer(position, sizeof(position));
		m_SpotLightData.IB.GenIndexBuffer(indices, 18);
		GLVertexBufferLayout layout;
		layout.Push<float>(3);
		m_SpotLightData.VA.AddBuffer(m_SpotLightData.VB, layout);
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
		m_DirLightData.VA.GenVertexArray();
		m_DirLightData.VB.GenVertexBuffer(position, sizeof(position));
		m_DirLightData.IB.GenIndexBuffer(indices, 36);
		GLVertexBufferLayout layout;
		layout.Push<float>(3);
		m_DirLightData.VA.AddBuffer(m_DirLightData.VB, layout);
	}
}

void GLData::Reset()
{
	m_ModelData.clear();
	GLCubeMap newCubeMap;
	m_SkyboxData.Skybox = newCubeMap;
}

void GLData::AddModel(std::string name, Model model)
{
	GLModelData modelData;
	m_ModelData.insert(std::pair<std::string, GLModelData>(name, modelData));
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	// Handle vertex position and index
	for (unsigned int i = 0; i < model.GetMeshes().size(); i++)
	{
		unsigned int indexOffset = (unsigned int)vertices.size();
		for (unsigned int j = 0; j < model.GetMeshes()[i].GetIndices().size(); j++)
		{
			indices.push_back(model.GetMeshes()[i].GetIndices()[j] + indexOffset);
		}
		// Handle materials
		m_ModelData[name].ReflectiveValue.push_back(model.GetMeshes()[i].GetReflectivenValue());
		m_ModelData[name].TransparentValue.push_back(model.GetMeshes()[i].GetTransparentValue());
		switch (model.GetStatics().RenderMode)
		{
		case NOTEX_HASCOLOR:
			m_ModelData[name].AmbientValue.push_back(model.GetMeshes()[i].GetAmbientValue());
			m_ModelData[name].DiffuseValue.push_back(model.GetMeshes()[i].GetDiffuseValue());
			m_ModelData[name].SpecularValue.push_back(model.GetMeshes()[i].GetSpecularValue());
			m_ModelData[name].EmissiveValue.push_back(model.GetMeshes()[i].GetEmissiveValue());
			for (unsigned int j = 0; j < model.GetMeshes()[i].GetVertices().size(); j++)
			{
				model.GetMeshes()[i].GetVertices()[j].MaterialIndex = glm::vec4(j);
			}
			break;
		default:
			break;
		}
		vertices.insert(vertices.end(), model.GetMeshes()[i].GetVertices().begin(), model.GetMeshes()[i].GetVertices().end());
	}
	m_ModelData[name].VA.GenVertexArray();
	m_ModelData[name].VB.GenVertexBuffer(&vertices[0], (unsigned int)vertices.size() * sizeof(Vertex));
	m_ModelData[name].IB.GenIndexBuffer(&indices[0], (unsigned int)indices.size());
	GLVertexBufferLayout layout;
	//vertex positions
	layout.Push<float>(3);
	//vertex normals
	layout.Push<float>(3);
	//vertex texture coords
	layout.Push<float>(2);
	//vertex material index
	layout.Push<float>(4);
	//vertex Normal, Bump or Displacement map index
	layout.Push<float>(3);
	//vertex tangent
	layout.Push<float>(3);
	//vertex bitangent
	layout.Push<float>(3);
	m_ModelData[name].VA.AddBuffer(m_ModelData[name].VB, layout);
	// Add model matrix as vertex attribute
	m_ModelData[name].InstanceVB.GenVertexBuffer(&model.GetModelMats()[0], (unsigned int)model.GetModelMats().size() * sizeof(glm::mat4));
	GLVertexBufferLayout instanceLayout;
	instanceLayout.Push<float>(4);
	instanceLayout.Push<float>(4);
	instanceLayout.Push<float>(4);
	instanceLayout.Push<float>(4);
	m_ModelData[name].VA.AddBuffer(m_ModelData[name].InstanceVB, instanceLayout, 1);
}

bool GLData::DeleteModel(std::string name)
{
	if (m_ModelData.find(name) != m_ModelData.end())
	{
		m_ModelData.erase(name);
		return true;
	}
	return false;
}

bool GLData::RenameModel(std::string oldName, std::string newName)
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

void GLData::AddLight(std::string name, Light_Type type)
{
	switch (type)
	{
	case POINT_LIGHT:
		AddPointLight(name);
		break;
	case SPOT_LIGHT:
		AddSpotLight(name);
		break;
	case DIRECTIONAL_LIGHT:
		AddDirLight(name);
		break;
	}
}

void GLData::DeleteLight(std::string name, Light_Type type)
{
	switch (type)
	{
	case POINT_LIGHT:
		DeletePointLight(name);
		break;
	case SPOT_LIGHT:
		DeleteSpotLight(name);
		break;
	case DIRECTIONAL_LIGHT:
		DeleteDirLight(name);
		break;
	}
}

void GLData::RenameLight(std::string oldName, std::string newName, Light_Type type)
{
	switch (type)
	{
	case POINT_LIGHT:
		RenamePointLight(oldName, newName);
		break;
	case SPOT_LIGHT:
		RenameSpotLight(oldName, newName);
		break;
	case DIRECTIONAL_LIGHT:
		RenameDirLight(oldName, newName);
		break;
	}
}

bool GLData::LoadSkybox(std::vector<std::string> filepath)
{
	if (m_SkyboxData.Skybox.GenTexture(filepath))
	{
		return true;
	}
	return false;
}

void GLData::AddPointLight(std::string name)
{
	GLFrameBuffer fb;
	m_PointLightData.DepthMap.insert(std::pair<std::string, GLFrameBuffer>(name, fb));
	m_PointLightData.DepthMap[name].Init(DEPTH_CUBE, 1024, 1024);
}

void GLData::AddSpotLight(std::string name)
{
	// TODO
}

void GLData::AddDirLight(std::string name)
{
	// TODO
}

void GLData::DeletePointLight(std::string name)
{
	m_PointLightData.DepthMap.erase(name);
}

void GLData::DeleteSpotLight(std::string name)
{
	// TODO
}

void GLData::DeleteDirLight(std::string name)
{
	// TODO
}

void GLData::RenamePointLight(std::string oldName, std::string newName)
{
	unsigned int width = m_PointLightData.DepthMap[oldName].GetTexWidth();
	unsigned int height = m_PointLightData.DepthMap[oldName].GetTexHeight();
	GLFrameBuffer fb;
	m_PointLightData.DepthMap.erase(oldName);
	m_PointLightData.DepthMap.insert(std::pair<std::string, GLFrameBuffer>(newName, fb));
	m_PointLightData.DepthMap[newName].Init(DEPTH_CUBE, width, height);
}

void GLData::RenameSpotLight(std::string oldName, std::string newName)
{
	// TODO
}

void GLData::RenameDirLight(std::string oldName, std::string newName)
{
	// TODO
}
