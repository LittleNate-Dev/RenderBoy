#include "GLData.h"

GLData::GLData()
{
}

GLData::~GLData()
{
}

void GLData::Init()
{
	// Init shaders
	m_PointLightData.Shader.Init(SHADER_OPENGL_SHADOW_POINT);
	m_SpotLightData.Shader.Init(SHADER_OPENGL_SHADOW_SPOT);
	m_DirLightData.Shader.Init(SHADER_OPENGL_SHADOW_DIR);
	m_SkyboxData.Shader.Init(SHADER_OPENGL_SKYBOX);
	m_CheckerMap.GenTexture(UV_MAP_FILEPATH);
	ChangeDrawMode();
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
	// Initialize data used for vfxs
	{
		// SSAO
		{
			// generates random floats between 0.0 and 1.0
			std::uniform_real_distribution<float> randomFloats(0.0, 1.0); 
			std::default_random_engine generator;
			for (unsigned int i = 0; i < 64; ++i)
			{
				glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
				sample = glm::normalize(sample);
				sample *= randomFloats(generator);
				float scale = float(i) / 64.0f;
				// scale samples s.t. they're more aligned to center of kernel
				scale = 0.1f + (scale * scale) * (1.0f - 0.1f);
				sample *= scale;
				m_VFXData.SSAOSamples.push_back(sample);
			}
		}
		m_VFXData.SSAONoiseTex.GenTexture(Noise_Tex_Type::NOISE_SSAO);
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
	ChangeDrawMode();
	m_ModelData.clear();
	m_PointLightData.DepthMap.clear();
	m_SpotLightData.DepthMap.clear();
	m_DirLightData.DepthMap.clear();
	GLCubeMap newCubeMap;
	m_SkyboxData.Skybox = newCubeMap;
}

void GLData::ChangeDrawMode()
{
	GLShader newShader;
	m_Shader = newShader;
	switch (core::SETTINGS.DrawMode)
	{
	case BLANK:
		m_Shader.Init(SHADER_OPENGL_BLANK);
		break;
	case WIREFRAME:
		m_Shader.Init(SHADER_OPENGL_WIREFRAME);
		break;
	case POINTCLOUD:
		m_Shader.Init(SHADER_OPENGL_POINTCLOUD);
		break;
	case UVSET:
		m_Shader.Init(SHADER_OPENGL_UVSET);
		m_Shader.Bind();
		m_Shader.SetUniformHandleARB("u_CheckerMap", m_CheckerMap.GetHandle());
		m_Shader.Unbind();
		break;
	case DEPTH:
		m_Shader.Init(SHADER_OPENGL_DEPTH);
		break;
	}
}

void GLData::AddModel(std::string name, Model& model)
{
	m_ModelList.push_back(name);
	GLModelData modelData;
	m_ModelData.insert(std::pair<std::string, GLModelData>(name, modelData));
	// Load in materials
	for (unsigned int i = 0; i < model.GetMeshes().size(); i++)
	{
		//std::cout << "Reflective value: " << model.GetMeshes()[i].GetReflectiveValue() << std::endl;
		bool hasValue = false;
		if (model.GetStatics().RenderMode == NOTEX)
		{
			// Ambient
			hasValue = false;
			for (unsigned int j = 0; j < m_ModelData[name].AmbientValue.size(); j++)
			{
				if (model.GetMeshes()[i].GetAmbientValue() == m_ModelData[name].AmbientValue[j])
				{
					hasValue = true;
					break;
				}
			}
			if (!hasValue)
			{
				m_ModelData[name].AmbientValue.push_back(model.GetMeshes()[i].GetAmbientValue());
			}
			// Diffuse
			hasValue = false;
			for (unsigned int j = 0; j < m_ModelData[name].DiffuseValue.size(); j++)
			{
				if (model.GetMeshes()[i].GetDiffuseValue() == m_ModelData[name].DiffuseValue[j])
				{
					hasValue = true;
					break;
				}
			}
			if (!hasValue)
			{
				m_ModelData[name].DiffuseValue.push_back(model.GetMeshes()[i].GetDiffuseValue());
			}
			// Specular
			hasValue = false;
			for (unsigned int j = 0; j < m_ModelData[name].SpecularValue.size(); j++)
			{
				if (model.GetMeshes()[i].GetSpecularValue() == m_ModelData[name].SpecularValue[j])
				{
					hasValue = true;
					break;
				}
			}
			if (!hasValue)
			{
				m_ModelData[name].SpecularValue.push_back(model.GetMeshes()[i].GetSpecularValue());
			}
		}
		// Reflective
		hasValue = false;
		for (unsigned int j = 0; j < m_ModelData[name].ReflectiveValue.size(); j++)
		{
			if (model.GetMeshes()[i].GetReflectiveValue() == m_ModelData[name].ReflectiveValue[j])
			{
				hasValue = true;
				break;
			}
		}
		if (!hasValue)
		{
			m_ModelData[name].ReflectiveValue.push_back(model.GetMeshes()[i].GetReflectiveValue());
		}
		// Transparent
		hasValue = false;
		for (unsigned int j = 0; j < m_ModelData[name].TransparentValue.size(); j++)
		{
			if (model.GetMeshes()[i].GetTransparentValue() == m_ModelData[name].TransparentValue[j])
			{
				hasValue = true;
				break;
			}
		}
		if (!hasValue)
		{
			m_ModelData[name].TransparentValue.push_back(model.GetMeshes()[i].GetTransparentValue());
		}
	}
	model.GetStatics().AmbientValueCount = m_ModelData[name].AmbientValue.size();
	model.GetStatics().DiffuseValueCount = m_ModelData[name].DiffuseValue.size();
	model.GetStatics().SpecularValueCount = m_ModelData[name].SpecularValue.size();
	model.GetStatics().ReflectiveCount = m_ModelData[name].ReflectiveValue.size();
	model.GetStatics().TransparentCount = m_ModelData[name].TransparentValue.size();
	m_ModelData[name].Statics = model.GetStatics();
	//std::cout << "Diffuse count: " << m_ModelData[name].Statics.DiffuseValueCount << std::endl;
	// Handle vertex position and index
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < model.GetMeshes().size(); i++)
	{
		unsigned int indexOffset = (unsigned int)vertices.size();
		for (unsigned int j = 0; j < model.GetMeshes()[i].GetIndices().size(); j++)
		{
			indices.push_back(model.GetMeshes()[i].GetIndices()[j] + indexOffset);
		}
		// Material index
		glm::vec4 texIndex = glm::vec4(-1);
		glm::vec3 colorIndex = glm::vec3(-1);
		glm::vec4 attributeIndex = glm::vec4(-1);
		glm::vec3 nbdIndex = glm::vec3(-1);
		if (model.GetStatics().RenderMode == NOTEX)
		{
			// Ambient
			for (unsigned int j = 0; j < m_ModelData[name].AmbientValue.size(); j++)
			{
				colorIndex.x++;
				if (model.GetMeshes()[i].GetAmbientValue() == m_ModelData[name].AmbientValue[j])
				{
					break;
				}
			}
			// Diffuse
			for (unsigned int j = 0; j < m_ModelData[name].DiffuseValue.size(); j++)
			{
				colorIndex.y++;
				if (model.GetMeshes()[i].GetDiffuseValue() == m_ModelData[name].DiffuseValue[j])
				{
					break;
				}
			}
			// Specular
			for (unsigned int j = 0; j < m_ModelData[name].SpecularValue.size(); j++)
			{
				colorIndex.z++;
				if (model.GetMeshes()[i].GetSpecularValue() == m_ModelData[name].SpecularValue[j])
				{
					break;
				}
			}
		}
		// Reflective
		for (unsigned int j = 0; j < m_ModelData[name].ReflectiveValue.size(); j++)
		{
			attributeIndex.x++;
			if (model.GetMeshes()[i].GetReflectiveValue() == m_ModelData[name].ReflectiveValue[j])
			{
				break;
			}
		}
		// Transparent
		for (unsigned int j = 0; j < m_ModelData[name].TransparentValue.size(); j++)
		{
			attributeIndex.y++;
			if (model.GetMeshes()[i].GetTransparentValue() == m_ModelData[name].TransparentValue[j])
			{
				break;
			}
		}
		for (unsigned int j = 0; j < model.GetMeshes()[i].GetVertices().size(); j++)
		{
			model.GetMeshes()[i].GetVertices()[j].ColorIndex = colorIndex;
			model.GetMeshes()[i].GetVertices()[j].AttributeIndex = attributeIndex;
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
	//vertex tangent
	layout.Push<float>(3);
	//vertex bitangent
	layout.Push<float>(3);
	//vertex texture index
	layout.Push<float>(4);
	//vertex color index
	layout.Push<float>(3);
	//mesh attribute index
	layout.Push<float>(4);
	//vertex Normal, Bump or Displacement map index
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
	// Init shader
	m_ModelData[name].Shader.Init(m_ModelData[name].Statics);
	m_ModelData[name].Shader.Bind();
	std::string uniformName;
	if (model.GetStatics().RenderMode == NOTEX)
	{
		for (unsigned int i = 0; i < m_ModelData[name].DiffuseValue.size(); i++)
		{
			uniformName = "u_Diffuse[" + std::to_string(i) + "]";
			m_ModelData[name].Shader.SetUniformVec3f(uniformName, m_ModelData[name].DiffuseValue[i]);
		}
		for (unsigned int i = 0; i < m_ModelData[name].SpecularValue.size(); i++)
		{
			uniformName = "u_Specular[" + std::to_string(i) + "]";
			m_ModelData[name].Shader.SetUniformVec3f(uniformName, m_ModelData[name].SpecularValue[i]);
		}
	}
	//for (unsigned int i = 0; i < m_ModelData[name].TransparentValue.size(); i++)
	//{
	//	uniformName = "u_Transparent[" + std::to_string(i) + "]";
	//	m_ModelData[name].Shader.SetUniform1f(uniformName, m_ModelData[name].TransparentValue[i]);
	//}
	m_ModelData[name].Shader.Unbind();
}

bool GLData::DeleteModel(std::string name)
{
	m_ModelList.erase(std::remove(m_ModelList.begin(), m_ModelList.end(), name), m_ModelList.end());
	m_ModelList.shrink_to_fit();
	if (m_ModelData.find(name) != m_ModelData.end())
	{
		m_ModelData.erase(name);
		return true;
	}
	return false;
}

bool GLData::RenameModel(std::string oldName, std::string newName)
{
	for (unsigned int i = 0; i < m_ModelList.size(); i++)
	{
		if (m_ModelList[i] == oldName)
		{
			m_ModelList[i] = newName;
			break;
		}
	}
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
		m_SkyboxData.Shader.Bind();
		m_SkyboxData.Shader.SetUniformHandleARB("u_Skybox", m_SkyboxData.Skybox.GetHandle());
		m_SkyboxData.Shader.Unbind();
		return true;
	}
	return false;
}

void GLData::SetShadowRes(std::string name, unsigned int width, unsigned int height, Light_Type type)
{
	switch (type)
	{
	case POINT_LIGHT:
		m_PointLightData.DepthMap[name].ChangeShadowRes(width, height);
		break;
	case SPOT_LIGHT:
		m_SpotLightData.DepthMap[name].ChangeShadowRes(width, height);
		break;
	case DIRECTIONAL_LIGHT:
		for (unsigned int i = 0; i < 3; i++)
		{
			m_DirLightData.DepthMap[name][i].ChangeShadowRes(width * (3 - i) / 3, height * (3 - i) / 3);
		}
		break;
	default:
		break;
	}
}

void GLData::AddPointLight(std::string name)
{
	GLFrameBuffer fb;
	m_PointLightData.DepthMap.insert(std::pair<std::string, GLFrameBuffer>(name, fb));
	m_PointLightData.DepthMap[name].Init(DEPTH_CUBE, 1024, 1024);
	ReInitShader();
}

void GLData::AddSpotLight(std::string name)
{
	GLFrameBuffer fb;
	m_SpotLightData.DepthMap.insert(std::pair<std::string, GLFrameBuffer>(name, fb));
	m_SpotLightData.DepthMap[name].Init(DEPTH_MAP, 1024, 1024);
	ReInitShader();
}

void GLData::AddDirLight(std::string name)
{
	GLFrameBuffer fb;
	std::vector<GLFrameBuffer> fbs;
	fbs.push_back(fb);
	fbs.push_back(fb);
	fbs.push_back(fb);
	m_DirLightData.DepthMap.insert(std::pair<std::string, std::vector<GLFrameBuffer>>(name, fbs));
	for (unsigned int i = 0; i < 3; i++)
	{
		m_DirLightData.DepthMap[name][i].Init(DEPTH_MAP, 1024 * (3 - i) / 3, 1024 * (3 - i) / 3);
	}
	ReInitShader();
}

void GLData::DeletePointLight(std::string name)
{
	m_PointLightData.DepthMap.erase(name);
	ReInitShader();
}

void GLData::DeleteSpotLight(std::string name)
{
	m_SpotLightData.DepthMap.erase(name);
	ReInitShader();
}

void GLData::DeleteDirLight(std::string name)
{
	m_DirLightData.DepthMap.erase(name);
	ReInitShader();
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
	unsigned int width = m_SpotLightData.DepthMap[oldName].GetTexWidth();
	unsigned int height = m_SpotLightData.DepthMap[oldName].GetTexHeight();
	GLFrameBuffer fb;
	m_SpotLightData.DepthMap.erase(oldName);
	m_SpotLightData.DepthMap.insert(std::pair<std::string, GLFrameBuffer>(newName, fb));
	m_SpotLightData.DepthMap[newName].Init(DEPTH_MAP, width, height);
}

void GLData::RenameDirLight(std::string oldName, std::string newName)
{
	unsigned int width[3], height[3];
	for (unsigned int i = 0; i < 3; i++)
	{
		width[i] = m_DirLightData.DepthMap[oldName][i].GetTexWidth();
		height[i] = m_DirLightData.DepthMap[oldName][i].GetTexHeight();
	}
	GLFrameBuffer fb;
	std::vector<GLFrameBuffer> fbs;
	fbs.push_back(fb);
	fbs.push_back(fb);
	fbs.push_back(fb);
	m_DirLightData.DepthMap.insert(std::pair<std::string, std::vector<GLFrameBuffer>>(newName, fbs));
	for (unsigned int i = 0; i < 3; i++)
	{
		m_DirLightData.DepthMap[newName][i].Init(DEPTH_MAP, width[i], height[i]);
	}
}

void GLData::ReInitShader()
{
	if (core::SETTINGS.DrawMode == BLANK)
	{
		m_Shader.Init(SHADER_OPENGL_BLANK);
	}
	std::string model;
	for (unsigned int i = 0; i < m_ModelList.size(); i++)
	{
		model = m_ModelList[i];
		m_ModelData[model].Shader.Init(m_ModelData[model].Statics);
		m_ModelData[model].Shader.Bind();
		std::string uniformName;
		if (m_ModelData[model].Statics.RenderMode == NOTEX)
		{
			for (unsigned int i = 0; i < m_ModelData[model].DiffuseValue.size(); i++)
			{
				uniformName = "u_Diffuse[" + std::to_string(i) + "]";
				m_ModelData[model].Shader.SetUniformVec3f(uniformName, m_ModelData[model].DiffuseValue[i]);
			}
			for (unsigned int i = 0; i < m_ModelData[model].SpecularValue.size(); i++)
			{
				uniformName = "u_Specular[" + std::to_string(i) + "]";
				m_ModelData[model].Shader.SetUniformVec3f(uniformName, m_ModelData[model].SpecularValue[i]);
			}
		}
		/*for (unsigned int i = 0; i < m_ModelData[model].TransparentValue.size(); i++)
		{
			uniformName = "u_Transparent[" + std::to_string(i) + "]";
			m_ModelData[model].Shader.SetUniform1f(uniformName, m_ModelData[model].TransparentValue[i]);
		}*/
		m_ModelData[model].Shader.Unbind();
	}
}
