#include "GLRenderer.h"

GLRenderer::GLRenderer()
{
}

GLRenderer::~GLRenderer()
{
}

bool GLRenderer::Init(Scene& scene)
{
	GLCall(glLineWidth(0.4f));
	GLCall(glPointSize(1.5f));
	// Initialize shaders
	if (!m_Shaders.Screen.Init(SHADER_OPENGL_SCREEN))
	{
		return false;
	}
	if (!m_Shaders.GBuffer.Init(SHADER_OPENGL_GBUFFER))
	{
		return false;
	}
	if (!m_Shaders.GBufferArea.Init(SHADER_OPENGL_GBUFFER_AREA))
	{
		return false;
	}
	if (!m_Shaders.Normal.Init(SHADER_OPENGL_NORMAL))
	{
		return false;
	}
	if (!m_Shaders.Lightcube.Init(SHADER_OPENGL_LIGHTCUBE))
	{
		return false;
	}
	if (!m_Shaders.LightcubeArea.Init(SHADER_OPENGL_LIGHTCUBE_AREA))
	{
		return false;
	}
	if (!m_Shaders.Bloom[0].Init(SHADER_OPENGL_BLOOM_DOWNSAMPLE))
	{
		return false;
	}
	if (!m_Shaders.Bloom[1].Init(SHADER_OPENGL_BLOOM_UPSAMPLE))
	{
		return false;
	}
	if (!m_Shaders.Bloom[2].Init(SHADER_OPENGL_BLOOM_BLEND))
	{
		return false;
	}
	if (!m_Shaders.OIT.Init(SHADER_OPENGL_OIT))
	{
		return false;
	}
	if (!m_Shaders.DOF[0].Init(SHADER_OPENGL_DOF_COC))
	{
		return false;
	}
	if (!m_Shaders.DOF[1].Init(SHADER_OPENGL_DOF_BOKEH))
	{
		return false;
	}
	if (!m_Shaders.DOF[2].Init(SHADER_OPENGL_DOF_DOWNSAMPLE))
	{
		return false;
	}
	if (!m_Shaders.DOF[3].Init(SHADER_OPENGL_DOF_BLEND))
	{
		return false;
	}
	if (!m_Shaders.GaussianBlur.Init(SHADER_OPENGL_UTIL_GAUSSIAN_BLUR))
	{
		return false;
	}
	if (!m_Shaders.FXAA.Init(SHADER_OPENGL_AA_FXAA))
	{
		return false;
	}
	if (!m_Shaders.Exposure[0].Init(SHADER_OPENGL_EXPOSURE_HISTOGRAM))
	{
		return false;
	}
	if (!m_Shaders.Exposure[1].Init(SHADER_OPENGL_EXPOSURE_AVERAGE))
	{
		return false;
	}
	if (!ChangePostProcess())
	{
		return false;
	}
	// Shaders used for SSAO
	{
		if (!m_Shaders.SSAO[0].Init(SHADER_OPENGL_SSAO_GEN))
		{
			return false;
		}
		m_Shaders.SSAO[0].Bind();
		for (unsigned int i = 0; i < 64; i++)
		{
			m_Shaders.SSAO[0].SetUniformVec3f("u_Samples[" + std::to_string(i) + "]", scene.GetData().GetDataGL().GetVFXData().SSAOSamples[i]);
		}
		m_Shaders.SSAO[0].SetUniformHandleARB("u_NoiseTex", scene.GetData().GetDataGL().GetVFXData().SSAONoiseTex.GetHandle());
		m_Shaders.SSAO[0].Unbind();
		if (!m_Shaders.SSAO[1].Init(SHADER_OPENGL_SSAO_BLUR))
		{
			return false;
		}
	}
	// Initialize frame buffers
	m_Frame.FB.Init(FBType::FRAME);
	m_Frame.Screen.Init(FBType::FRAME);
	m_Frame.GBuffer.Init(FBType::G_BUFFER);
	m_Frame.OIT.Init(FBType::OIT);
	// Initialize framebuffer used for FXAA
	m_Frame.FXAA.Init(FBType::FRAME);
	// Initialize framebuffers used for SSAO
	m_Frame.SSAO[0].Init(FBType::SSAO); 
	m_Frame.SSAO[1].Init(FBType::SSAO);
	// Initialize framebuffers used for depth of field
	m_Frame.DOF[0].Init(FBType::FRAME);
	m_Frame.DOF[1].Init(FBType::FRAME);
	glm::vec2 renderRes = core::GetRenderRes() * 0.5f;
	m_Frame.DOF[2].Init(FBType::FRAME, (int)renderRes.x, (int)renderRes.y);
	m_Frame.DOF[3].Init(FBType::FRAME, (int)renderRes.x, (int)renderRes.y);
	// Initialize framebuffers used for bloom effect
	int bloomWidth = (int)(core::SETTINGS.Width * core::SETTINGS.Resolution);
	int bloomHeight = (int)(core::SETTINGS.Height * core::SETTINGS.Resolution);
	m_Frame.Bloom[6].Init(FBType::FRAME, bloomWidth, bloomHeight);
	for (unsigned int i = 0; i < 6; i++)
	{
		if (bloomWidth > 6 && bloomHeight > 6)
		{
			m_Frame.Bloom[i].Init(FBType::FRAME, bloomWidth, bloomHeight);
			bloomWidth /= 2;
			bloomHeight /= 2;
		}
		else
		{
			break;
		}
	}
	if (m_Frame.FB.IsInitialized())
	{
		// Draw a quad on the screen and put framebuffer on it
		float position[] = {
			// positions   // texCoords
			 1.0f,  1.0f,  1.0f, 1.0f, //0
			-1.0f,  1.0f,  0.0f, 1.0f, //1
			-1.0f, -1.0f,  0.0f, 0.0f, //2
			 1.0f, -1.0f,  1.0f, 0.0f  //3
		};
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		m_Frame.VA.GenVertexArray();
		m_Frame.VB.GenBuffer(position, sizeof(position));
		m_Frame.IB.GenBuffer(indices, 6);
		GLVertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_Frame.VA.AddBuffer(m_Frame.VB, layout);
	}
	else
	{
		core::ShowWarningMsg("Failed to initialize OpenGL renderer!");
		return false;
	}	
	return true;
}

void GLRenderer::UpdateModelMat(Scene& scene)
{
	std::string model;
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].InstanceVB.UpdateBuffer(
			&scene.GetModels()[model].GetModelMats()[0],
			(unsigned int)scene.GetModels()[model].GetModelMats().size() * sizeof(glm::mat4)
		);
	}
}

void GLRenderer::Clear()
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void GLRenderer::Draw(Scene& scene)
{
	UpdateModelMat(scene);
	DrawGBuffer(scene);
	if (scene.GetVFX().SSAO)
	{
		DrawSSAO(scene);
	}
	// Draw Shadow Depth map
	if (core::SETTINGS.DrawMode == DEFAULT || core::SETTINGS.DrawMode == BLANK)
	{
		std::string model;
		bool update = false;
		static int modelCount = 0;
		if (modelCount != scene.GetModelList().size())
		{
			update = true;
			modelCount = scene.GetModelList().size();
		}
		for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
		{
			model = scene.GetModelList()[i];
			update |= scene.GetModels()[model].UpdateShadow();
			scene.GetModels()[model].UpdateShadow() = false;
		}
		DrawPointLightShadow(scene, update);
		DrawSpotLightShadow(scene, update);
		DrawDirLightShadow(scene);
	}
	// Draw your content inside this scope
	m_Frame.FB.Bind();
	Clear();
	m_Frame.FB.Unbind();
	glm::vec2 renderRes = core::GetRenderRes();
	GLCall(glViewport(0, 0, (GLsizei)renderRes.x, (GLsizei)renderRes.y));
	switch (core::SETTINGS.DrawMode)
	{
	case DEFAULT:
		DrawDefault(scene);
		break;
	case BLANK:
		DrawBlank(scene);
		break;
	case WIREFRAME:
		DrawWireFrame(scene);
		break;
	case POINTCLOUD:
		DrawPointCloud(scene);
		break;
	case DEPTH:
		DrawDepth(scene);
		break;
	case UVSET:
		DrawUVSet(scene);
		break;
	case NORMAL_DM:
		DrawNormalDM(scene);
		break;
	}
	// Draw normals
	if (core::SETTINGS.ShowNormal)
	{
		DrawNormal(scene);
	}
	m_Frame.FB.Unbind();
	Clear();
	// Draw frame buffer's content on the screen;
	GLCall(glDisable(GL_DEPTH_TEST));
	GLCall(glViewport(0, 0, core::SETTINGS.Width, core::SETTINGS.Height));
	m_Shaders.PP.Bind();
	m_Shaders.PP.SetUniformHandleARB("u_ScreenTex", m_Frame.FB.GetHandle());
	//m_Shaders.PP.SetUniformHandleARB("u_ScreenTex", m_Frame.Bloom[0].GetHandle());
	m_Frame.VA.Bind();
	m_Frame.IB.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	m_Frame.VA.Unbind();
	m_Frame.IB.Unbind();
	m_Shaders.PP.Unbind();
	GLCall(glEnable(GL_DEPTH_TEST));
}

void GLRenderer::DrawGBuffer(Scene& scene)
{
	m_Frame.GBuffer.Bind();
	Clear();
	glm::vec2 texSize = m_Frame.GBuffer.GetTexSize();
	GLCall(glViewport(0, 0, texSize.x, texSize.y));
	m_Shaders.GBuffer.Bind();
	m_Shaders.GBuffer.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.GBuffer.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	m_Shaders.GBuffer.SetUniformVec2f("u_Plane", scene.GetCamera().GetPlane());
	std::string model;
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();
		// Draw Model
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
	}	
	m_Shaders.GBuffer.Unbind();
	// Draw Area light's cube
	std::string light;
	m_Shaders.GBufferArea.Bind();
	m_Shaders.GBufferArea.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.GBufferArea.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	m_Shaders.GBufferArea.SetUniformVec2f("u_Plane", scene.GetCamera().GetPlane());
	for (unsigned int i = 0; i < scene.GetAreaLightList().size(); i++)
	{
		light = scene.GetAreaLightList()[i];
		if (scene.GetAreaLights()[light].LightSwitch() && scene.GetAreaLights()[light].ShowCube())
		{
			m_Shaders.GBufferArea.SetUniformMat4f("u_ModelMat", scene.GetAreaLights()[light].GetModelMat());
			DrawAreaLightCube(scene, scene.GetAreaLights()[light].GetLightType());
		}
	}
	m_Shaders.GBufferArea.Unbind();
	m_Frame.GBuffer.Unbind();
}

void GLRenderer::DrawDefault(Scene& scene)
{
	// Draw Skybox
	DrawSkybox(scene);
	std::string model;
	// 1.Opaque pass
	m_Frame.FB.Bind();
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LESS));
	GLCall(glDepthMask(GL_TRUE));
	GLCall(glDisable(GL_BLEND));
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].Shader.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
		scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
		scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f("u_ViewPos", scene.GetCamera().GetPosition());
		scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1i("u_SSAO", scene.GetVFX().SSAO);
		scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformHandleARB("u_SSAOTex", m_Frame.SSAO[1].GetHandle());
		scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformHandleARB("u_ShadowOffset", scene.GetData().GetDataGL().GetSpotLightData().ShadowOffset.GetHandle());
		std::string light;
		// Set point lights' uniforms
		for (unsigned int j = 0; j < scene.GetPointLightList().size(); j++)
		{
			light = scene.GetPointLightList()[j];
			std::string lightName = "u_PointLight[" + std::to_string(j) + "].";
			scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1i(lightName + "LightSwitch", scene.GetPointLights()[light].LightSwitch());
			if (scene.GetPointLights()[light].LightSwitch())
			{
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "Position", scene.GetPointLights()[light].GetPosition());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "Color", scene.GetPointLights()[light].GetColor());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "ADS", scene.GetPointLights()[light].GetADS());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "CLQ", scene.GetPointLights()[light].GetCLQ());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1f(lightName + "Range", scene.GetPointLights()[light].GetRange());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1f(lightName + "Intensity", scene.GetPointLights()[light].GetIntensity());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1i(lightName + "CastShadow", scene.GetPointLights()[light].CastShadow());
				if (scene.GetPointLights()[light].CastShadow())
				{
					scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1i(lightName + "SoftShadow", scene.GetPointLights()[light].SoftShadow());
					scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformHandleARB(lightName + "ShadowMap", scene.GetData().GetDataGL().GetPointLightData().DepthMap[light].GetHandle());
					scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1f(lightName + "Bias", scene.GetPointLights()[light].GetBias());
					scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1f(lightName + "FarPlane", scene.GetPointLights()[light].GetFarPlane());
					scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1f(lightName + "SoftDegree", scene.GetPointLights()[light].GetSoftDegree());
				}
			}
		}
		// Set spot lights' uniforms
		for (unsigned int j = 0; j < scene.GetSpotLightList().size(); j++)
		{
			light = scene.GetSpotLightList()[j];
			std::string lightName = "u_SpotLight[" + std::to_string(j) + "].";
			scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1i(lightName + "LightSwitch", scene.GetSpotLights()[light].LightSwitch());
			if (scene.GetPointLights()[light].LightSwitch())
			{
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformMat4f("u_SpotMat[" + std::to_string(j) + "].ProjMat", scene.GetSpotLights()[light].GetProjMat());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformMat4f("u_SpotMat[" + std::to_string(j) + "].ViewMat", scene.GetSpotLights()[light].GetViewMat());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "Position", scene.GetSpotLights()[light].GetPosition());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "Direction", scene.GetSpotLights()[light].GetDirection());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1f(lightName + "Angle", glm::radians(scene.GetSpotLights()[light].GetAngle()));
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1f(lightName + "DimAngle", glm::radians(scene.GetSpotLights()[light].GetDimAngle()));
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "Color", scene.GetSpotLights()[light].GetColor());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "ADS", scene.GetSpotLights()[light].GetADS());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "CLQ", scene.GetSpotLights()[light].GetCLQ());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1f(lightName + "Range", scene.GetSpotLights()[light].GetRange());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1f(lightName + "Intensity", scene.GetSpotLights()[light].GetIntensity());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1i(lightName + "CastShadow", scene.GetSpotLights()[light].CastShadow());
				if (scene.GetSpotLights()[light].CastShadow())
				{
					scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1i(lightName + "SoftShadow", scene.GetSpotLights()[light].SoftShadow());
					scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformHandleARB(lightName + "ShadowMap", scene.GetData().GetDataGL().GetSpotLightData().DepthMap[light].GetHandle());
					scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec2f(lightName + "Bias", scene.GetSpotLights()[light].GetBias());
					scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1f(lightName + "FarPlane", scene.GetSpotLights()[light].GetFarPlane());
					scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1f(lightName + "SoftDegree", scene.GetSpotLights()[light].GetSoftDegree());
				}
			}
		}
		// Set dir lights' uniforms
		for (unsigned int j = 0; j < scene.GetDirLightList().size(); j++)
		{
			light = scene.GetDirLightList()[j];
			std::string lightName = "u_DirLight[" + std::to_string(j) + "].";
			scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1i(lightName + "LightSwitch", scene.GetDirLights()[light].LightSwitch());
			if (scene.GetDirLights()[light].LightSwitch())
			{
				for (unsigned int k = 0; k < 3; k++)
				{
					scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformMat4f("u_DirMat[" + std::to_string(j) + "].ProjMat[" + std::to_string(k) + "]", scene.GetDirLights()[light].GetProjMat(k));
					scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformMat4f("u_DirMat[" + std::to_string(j) + "].ViewMat[" + std::to_string(k) + "]", scene.GetDirLights()[light].GetViewMat(k));
				}
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "Direction", scene.GetDirLights()[light].GetDirection());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "Color", scene.GetDirLights()[light].GetColor());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "ADS", scene.GetDirLights()[light].GetADS());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1f(lightName + "Intensity", scene.GetDirLights()[light].GetIntensity());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1i(lightName + "CastShadow", scene.GetDirLights()[light].CastShadow());
				if (scene.GetDirLights()[light].CastShadow())
				{
					for (unsigned int k = 0; k < 3; k++)
					{
						scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformHandleARB(lightName + "ShadowMap[" + std::to_string(k) + "]", scene.GetData().GetDataGL().GetDirLightData().DepthMap[light][k].GetHandle());
					}
					scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "Bias", scene.GetDirLights()[light].GetBias());
					scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1i(lightName + "SoftShadow", scene.GetDirLights()[light].SoftShadow());
					scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1f(lightName + "SoftDegree", scene.GetDirLights()[light].GetSoftDegree());
				}
			}
		}
		// Set area lights' uniforms
		scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformHandleARB("u_LTC1", scene.GetData().GetDataGL().GetAreaLightData().LTC1.GetHandle());
		scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformHandleARB("u_LTC2", scene.GetData().GetDataGL().GetAreaLightData().LTC2.GetHandle());
		for (unsigned int j = 0; j < scene.GetAreaLightList().size(); j++)
		{
			light = scene.GetAreaLightList()[j];
			std::string lightName = "u_AreaLight[" + std::to_string(j) + "].";
			scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1i(lightName + "LightSwitch", scene.GetAreaLights()[light].LightSwitch());
			if (scene.GetAreaLights()[light].LightSwitch())
			{
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1i(lightName + "Type", scene.GetAreaLights()[light].GetLightType()); 
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "Position", scene.GetAreaLights()[light].GetPosition());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "Scale", scene.GetAreaLights()[light].GetScale());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "Points[0]", scene.GetAreaLights()[light].GetPoints(0));
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "Points[1]", scene.GetAreaLights()[light].GetPoints(1));
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "Points[2]", scene.GetAreaLights()[light].GetPoints(2));
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "Points[3]", scene.GetAreaLights()[light].GetPoints(3));
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1f(lightName + "Intensity", scene.GetAreaLights()[light].GetIntensity());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniformVec3f(lightName + "Color", scene.GetAreaLights()[light].GetColor());
				scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1i(lightName + "TwoSided", scene.GetAreaLights()[light].TwoSided());
			}
		}
		scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();		
		scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1i("u_OITPass", 0);
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].Shader.Unbind();
	}
	// Draw Area light's cube
	m_Shaders.LightcubeArea.Bind();
	m_Shaders.LightcubeArea.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.LightcubeArea.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	std::string light;
	for (unsigned int i = 0; i < scene.GetAreaLightList().size(); i++)
	{
		light = scene.GetAreaLightList()[i];
		if (scene.GetAreaLights()[light].LightSwitch() && scene.GetAreaLights()[light].ShowCube())
		{
			m_Shaders.LightcubeArea.SetUniformMat4f("u_ModelMat", scene.GetAreaLights()[light].GetModelMat());
			m_Shaders.LightcubeArea.SetUniformVec3f("u_Color", scene.GetAreaLights()[light].GetColor());
			m_Shaders.LightcubeArea.SetUniform1f("u_Intensity", scene.GetAreaLights()[light].GetIntensity());
			DrawAreaLightCube(scene, scene.GetAreaLights()[light].GetLightType());
		}
	}
	m_Shaders.LightcubeArea.Unbind();
	m_Frame.FB.Unbind();
	// 2.Transparent pass
	m_Frame.OIT.Bind();
	int width = m_Frame.FB.GetTexWidth();
	int height = m_Frame.FB.GetTexHeight();
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Frame.FB.GetID()));
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Frame.OIT.GetID()));
	GLCall(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST));
	GLCall(glDepthMask(GL_FALSE));
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunci(0, GL_ONE, GL_ONE));
	GLCall(glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR));
	GLCall(glBlendEquation(GL_FUNC_ADD));
	GLCall(glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.0f)[0]));
	GLCall(glClearBufferfv(GL_COLOR, 1, &glm::vec4(1.0f)[0]));
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		if (scene.GetModels()[model].GetStatics().RenderMode == Render_Mode::NOTEX)
		{
			continue;
		}
		scene.GetData().GetDataGL().GetModelData()[model].Shader.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].Shader.SetUniform1i("u_OITPass", 1);
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].Shader.Unbind();
	}
	m_Frame.OIT.Unbind();
	// 3.Composite the result
	m_Frame.FB.Bind();
	GLCall(glDepthFunc(GL_ALWAYS));
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	m_Shaders.OIT.Bind();
	m_Shaders.OIT.SetUniformHandleARB("u_Accum", m_Frame.OIT.GetHandle(0));
	m_Shaders.OIT.SetUniformHandleARB("u_Reveal", m_Frame.OIT.GetHandle(1));
	m_Frame.VA.Bind();
	m_Frame.IB.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	m_Frame.VA.Unbind();
	m_Frame.IB.Unbind();
	m_Shaders.OIT.Unbind();
	m_Frame.FB.Unbind();
	GLCall(glDepthMask(GL_TRUE));
	GLCall(glDepthFunc(GL_LESS));
	// Auto Exposure
	if (scene.GetCamera().GetExposure().Auto)
	{
		DrawAutoExposure(scene);
	}
	// Apply FXAA
	if (core::SETTINGS.AA == FXAA)
	{
		DrawFXAA(scene);
	}
	// Draw Bloom
	if (scene.GetCamera().GetBloom().Switch)
	{
		DrawBloom(scene);
	}
	// Tone mapping
	DrawToneMapping(scene);
	// Draw Depth of Field
	if (scene.GetCamera().GetFocus().Switch)
	{
		DrawDOF(scene);
	}
	// Draw Light Cube
	DrawLightCube(scene);
}

void GLRenderer::DrawBlank(Scene& scene)
{
	m_Frame.FB.Bind();
	scene.GetData().GetDataGL().GetShader().Bind();
	scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	scene.GetData().GetDataGL().GetShader().SetUniformVec3f("u_ViewPos", scene.GetCamera().GetPosition());
	scene.GetData().GetDataGL().GetShader().SetUniform1i("u_SSAO", scene.GetVFX().SSAO);
	scene.GetData().GetDataGL().GetShader().SetUniformHandleARB("u_SSAOTex", m_Frame.SSAO[1].GetHandle());
	std::string light;
	// Set point lights' uniforms
	for (unsigned int i = 0; i < scene.GetPointLightList().size(); i++)
	{
		light = scene.GetPointLightList()[i];
		std::string lightName = "u_PointLight[" + std::to_string(i) + "].";
		scene.GetData().GetDataGL().GetShader().SetUniform1i(lightName + "LightSwitch", scene.GetPointLights()[light].LightSwitch());
		if (scene.GetPointLights()[light].LightSwitch())
		{
			scene.GetData().GetDataGL().GetShader().SetUniformVec3f(lightName + "Position", scene.GetPointLights()[light].GetPosition());
			scene.GetData().GetDataGL().GetShader().SetUniformVec3f(lightName + "Color", scene.GetPointLights()[light].GetColor());
			scene.GetData().GetDataGL().GetShader().SetUniformVec3f(lightName + "ADS", scene.GetPointLights()[light].GetADS());
			scene.GetData().GetDataGL().GetShader().SetUniformVec3f(lightName + "CLQ", scene.GetPointLights()[light].GetCLQ());
			scene.GetData().GetDataGL().GetShader().SetUniform1f(lightName + "Range", scene.GetPointLights()[light].GetRange());
			scene.GetData().GetDataGL().GetShader().SetUniform1f(lightName + "Intensity", scene.GetPointLights()[light].GetIntensity());
			scene.GetData().GetDataGL().GetShader().SetUniform1i(lightName + "CastShadow", scene.GetPointLights()[light].CastShadow());
			if (scene.GetPointLights()[light].CastShadow())
			{
				scene.GetData().GetDataGL().GetShader().SetUniform1i(lightName + "SoftShadow", scene.GetPointLights()[light].SoftShadow());
				scene.GetData().GetDataGL().GetShader().SetUniformHandleARB(lightName + "ShadowMap", scene.GetData().GetDataGL().GetPointLightData().DepthMap[light].GetHandle());
				scene.GetData().GetDataGL().GetShader().SetUniform1f(lightName + "Bias", scene.GetPointLights()[light].GetBias());
				scene.GetData().GetDataGL().GetShader().SetUniform1f(lightName + "FarPlane", scene.GetPointLights()[light].GetFarPlane());
				scene.GetData().GetDataGL().GetShader().SetUniform1f(lightName + "SoftDegree", scene.GetPointLights()[light].GetSoftDegree());
			}
		}
	}
	// Set spot lights' uniforms
	for (unsigned int i = 0; i < scene.GetSpotLightList().size(); i++)
	{
		light = scene.GetSpotLightList()[i];
		std::string lightName = "u_SpotLight[" + std::to_string(i) + "].";
		scene.GetData().GetDataGL().GetShader().SetUniform1i(lightName + "LightSwitch", scene.GetSpotLights()[light].LightSwitch());
		if (scene.GetPointLights()[light].LightSwitch())
		{
			scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_SpotMat[" + std::to_string(i) + "].ProjMat", scene.GetSpotLights()[light].GetProjMat());
			scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_SpotMat[" + std::to_string(i) + "].ViewMat", scene.GetSpotLights()[light].GetViewMat());
			scene.GetData().GetDataGL().GetShader().SetUniformVec3f(lightName + "Position", scene.GetSpotLights()[light].GetPosition());
			scene.GetData().GetDataGL().GetShader().SetUniformVec3f(lightName + "Direction", scene.GetSpotLights()[light].GetDirection());
			scene.GetData().GetDataGL().GetShader().SetUniform1f(lightName + "Angle", glm::radians(scene.GetSpotLights()[light].GetAngle()));
			scene.GetData().GetDataGL().GetShader().SetUniform1f(lightName + "DimAngle", glm::radians(scene.GetSpotLights()[light].GetDimAngle()));
			scene.GetData().GetDataGL().GetShader().SetUniformVec3f(lightName + "Color", scene.GetSpotLights()[light].GetColor());
			scene.GetData().GetDataGL().GetShader().SetUniformVec3f(lightName + "ADS", scene.GetSpotLights()[light].GetADS());
			scene.GetData().GetDataGL().GetShader().SetUniformVec3f(lightName + "CLQ", scene.GetSpotLights()[light].GetCLQ());
			scene.GetData().GetDataGL().GetShader().SetUniform1f(lightName + "Range", scene.GetSpotLights()[light].GetRange());
			scene.GetData().GetDataGL().GetShader().SetUniform1f(lightName + "Intensity", scene.GetSpotLights()[light].GetIntensity());
			scene.GetData().GetDataGL().GetShader().SetUniform1i(lightName + "CastShadow", scene.GetSpotLights()[light].CastShadow());
			if (scene.GetSpotLights()[light].CastShadow())
			{
				scene.GetData().GetDataGL().GetShader().SetUniform1i(lightName + "SoftShadow", scene.GetSpotLights()[light].SoftShadow());
				scene.GetData().GetDataGL().GetShader().SetUniformHandleARB(lightName + "ShadowMap", scene.GetData().GetDataGL().GetSpotLightData().DepthMap[light].GetHandle());
				scene.GetData().GetDataGL().GetShader().SetUniformVec2f(lightName + "Bias", scene.GetSpotLights()[light].GetBias());
				scene.GetData().GetDataGL().GetShader().SetUniform1f(lightName + "FarPlane", scene.GetSpotLights()[light].GetFarPlane());
				scene.GetData().GetDataGL().GetShader().SetUniform1f(lightName + "SoftDegree", scene.GetSpotLights()[light].GetSoftDegree());
			}
		}
	}
	// Set dir lights' uniforms
	for (unsigned int i = 0; i < scene.GetDirLightList().size(); i++)
	{
		light = scene.GetDirLightList()[i];
		std::string lightName = "u_DirLight[" + std::to_string(i) + "].";
		scene.GetData().GetDataGL().GetShader().SetUniform1i(lightName + "LightSwitch", scene.GetDirLights()[light].LightSwitch());
		if (scene.GetDirLights()[light].LightSwitch())
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_DirMat[" + std::to_string(i) + "].ProjMat[" + std::to_string(j) + "]", scene.GetDirLights()[light].GetProjMat(j));
				scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_DirMat[" + std::to_string(i) + "].ViewMat[" + std::to_string(j) + "]", scene.GetDirLights()[light].GetViewMat(j));
			}
			scene.GetData().GetDataGL().GetShader().SetUniformVec3f(lightName + "Direction", scene.GetDirLights()[light].GetDirection());
			scene.GetData().GetDataGL().GetShader().SetUniformVec3f(lightName + "Color", scene.GetDirLights()[light].GetColor());
			scene.GetData().GetDataGL().GetShader().SetUniformVec3f(lightName + "ADS", scene.GetDirLights()[light].GetADS());
			scene.GetData().GetDataGL().GetShader().SetUniform1f(lightName + "Intensity", scene.GetDirLights()[light].GetIntensity());
			scene.GetData().GetDataGL().GetShader().SetUniform1i(lightName + "CastShadow", scene.GetDirLights()[light].CastShadow());
			if (scene.GetDirLights()[light].CastShadow())
			{
				for (unsigned int j = 0; j < 3; j++)
				{
					scene.GetData().GetDataGL().GetShader().SetUniformHandleARB(lightName + "ShadowMap[" + std::to_string(j) + "]", scene.GetData().GetDataGL().GetDirLightData().DepthMap[light][j].GetHandle());
				}
				scene.GetData().GetDataGL().GetShader().SetUniformVec3f(lightName + "Bias", scene.GetDirLights()[light].GetBias());
				scene.GetData().GetDataGL().GetShader().SetUniform1i(lightName + "SoftShadow", scene.GetDirLights()[light].SoftShadow());
				scene.GetData().GetDataGL().GetShader().SetUniform1f(lightName + "SoftDegree", scene.GetDirLights()[light].GetSoftDegree());
			}
		}
	}
	std::string model;
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();
		// Draw Model
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
	}
	scene.GetData().GetDataGL().GetShader().Unbind();
	// Draw Area light's cube
	m_Shaders.LightcubeArea.Bind();
	m_Shaders.LightcubeArea.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.LightcubeArea.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	for (unsigned int i = 0; i < scene.GetAreaLightList().size(); i++)
	{
		light = scene.GetAreaLightList()[i];
		if (scene.GetAreaLights()[light].LightSwitch() && scene.GetAreaLights()[light].ShowCube())
		{
			m_Shaders.LightcubeArea.SetUniformMat4f("u_ModelMat", scene.GetAreaLights()[light].GetModelMat());
			m_Shaders.LightcubeArea.SetUniformVec3f("u_Color", scene.GetAreaLights()[light].GetColor());
			m_Shaders.LightcubeArea.SetUniform1f("u_Intensity", scene.GetAreaLights()[light].GetIntensity());
			DrawAreaLightCube(scene, scene.GetAreaLights()[light].GetLightType());
		}
	}
	m_Shaders.LightcubeArea.Unbind();
	m_Frame.FB.Unbind();
	// Auto Exposure
	if (scene.GetCamera().GetExposure().Auto)
	{
		DrawAutoExposure(scene);
	}
	// Apply FXAA
	if (core::SETTINGS.AA == FXAA)
	{
		DrawFXAA(scene);
	}
	// Draw Bloom
	if (scene.GetCamera().GetBloom().Switch)
	{
		DrawBloom(scene);
	}
	// Tone mapping
	DrawToneMapping(scene);
	// Draw Depth of Field
	if (scene.GetCamera().GetFocus().Switch)
	{
		DrawDOF(scene);
	}
	// Draw Light Cube
	DrawLightCube(scene);
}

void GLRenderer::DrawWireFrame(Scene& scene)
{
	m_Frame.FB.Bind();
	std::string model;
	scene.GetData().GetDataGL().GetShader().Bind();
	scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	glm::vec2 renderRes = core::GetRenderRes();
	scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_ViewPortMat", core::GetViewPortMatrix((unsigned int)renderRes.x, (unsigned int)renderRes.y));
	scene.GetData().GetDataGL().GetShader().SetUniformVec2f("u_RenderRes", renderRes);
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();
		// Draw Model
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
	}
	scene.GetData().GetDataGL().GetShader().Unbind();
	m_Frame.FB.Unbind();
	// Apply FXAA
	if (core::SETTINGS.AA == FXAA)
	{
		DrawFXAA(scene);
	}
}

void GLRenderer::DrawPointCloud(Scene& scene)
{
	m_Frame.FB.Bind();
	std::string model;
	scene.GetData().GetDataGL().GetShader().Bind();
	scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();
		GLCall(glDrawElementsInstanced(GL_POINTS, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
	}
	scene.GetData().GetDataGL().GetShader().Unbind();
	m_Frame.FB.Unbind();
	// Apply FXAA
	if (core::SETTINGS.AA == FXAA)
	{
		DrawFXAA(scene);
	}
}

void GLRenderer::DrawDepth(Scene& scene)
{
	m_Frame.FB.Bind();
	std::string model;
	scene.GetData().GetDataGL().GetShader().Bind();
	scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	scene.GetData().GetDataGL().GetShader().SetUniform1f("u_NearPlane", scene.GetCamera().GetNearPlane());
	scene.GetData().GetDataGL().GetShader().SetUniform1f("u_FarPlane", scene.GetCamera().GetFarPlane());
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
	}
	scene.GetData().GetDataGL().GetShader().Unbind();
	m_Frame.FB.Unbind();
	// Apply FXAA
	if (core::SETTINGS.AA == FXAA)
	{
		DrawFXAA(scene);
	}
}

void GLRenderer::DrawNormalDM(Scene& scene)
{
	m_Frame.FB.Bind();
	std::string model;
	scene.GetData().GetDataGL().GetShader().Bind();
	scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
	}
	scene.GetData().GetDataGL().GetShader().Unbind();
	m_Frame.FB.Unbind();
	// Apply FXAA
	if (core::SETTINGS.AA == FXAA)
	{
		DrawFXAA(scene);
	}
}

void GLRenderer::DrawUVSet(Scene& scene)
{
	m_Frame.FB.Bind();
	std::string model;
	scene.GetData().GetDataGL().GetShader().Bind();
	scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();
		// Draw Model
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
	}
	scene.GetData().GetDataGL().GetShader().Unbind();
	m_Frame.FB.Unbind();
	// Apply FXAA
	if (core::SETTINGS.AA == FXAA)
	{
		DrawFXAA(scene);
	}
	float exposure = scene.GetCamera().GetExposure().Strength;
	scene.GetCamera().SetExposure(1.0f);
	// Tone mapping
	DrawToneMapping(scene);
	scene.GetCamera().SetExposure(exposure);
}

void GLRenderer::DrawNormal(Scene& scene)
{
	m_Frame.FB.Bind();
	std::string model;
	m_Shaders.Normal.Bind();
	m_Shaders.Normal.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.Normal.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	m_Shaders.Normal.SetUniform1f("u_Magnitude", core::SETTINGS.NormalMagnitude);
	m_Shaders.Normal.SetUniformVec3f("u_Color", core::SETTINGS.NormalColor);
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
	}
	m_Shaders.Normal.Unbind();
	m_Frame.FB.Bind();
}

void GLRenderer::DrawLightCube(Scene& scene)
{
	m_Frame.FB.Bind();
	m_Shaders.Lightcube.Bind();
	glm::vec2 renderRes = core::GetRenderRes();
	m_Shaders.Lightcube.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.Lightcube.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	m_Shaders.Lightcube.SetUniformMat4f("u_ViewPortMat", core::GetViewPortMatrix((unsigned int)renderRes.x, (unsigned int)renderRes.y));
	m_Shaders.Lightcube.SetUniformVec2f("u_RenderRes", renderRes);

	std::string light;
	// Draw Point light's cube
	scene.GetData().GetDataGL().GetPointLightData().VA.Bind();
	scene.GetData().GetDataGL().GetPointLightData().IB.Bind();
	for (unsigned int i = 0; i < scene.GetPointLightList().size(); i++)
	{
		light = scene.GetPointLightList()[i];
		if (scene.GetPointLights()[light].LightSwitch() && scene.GetPointLights()[light].ShowCube())
		{
			m_Shaders.Lightcube.SetUniformMat4f("u_ModelMat", scene.GetPointLights()[light].GetModelMat());
			m_Shaders.Lightcube.SetUniformVec3f("u_Color", scene.GetPointLights()[light].GetColor());
			GLCall(glDrawElements(GL_TRIANGLES, scene.GetData().GetDataGL().GetPointLightData().IB.GetCount(), GL_UNSIGNED_INT, nullptr));
		}
	}
	scene.GetData().GetDataGL().GetPointLightData().VA.Unbind();
	scene.GetData().GetDataGL().GetPointLightData().IB.Unbind();
	// Draw Spot light's cube
	scene.GetData().GetDataGL().GetSpotLightData().VA.Bind();
	scene.GetData().GetDataGL().GetSpotLightData().IB.Bind();
	for (unsigned int i = 0; i < scene.GetSpotLightList().size(); i++)
	{
		light = scene.GetSpotLightList()[i];
		if (scene.GetSpotLights()[light].LightSwitch() && scene.GetSpotLights()[light].ShowCube())
		{
			m_Shaders.Lightcube.SetUniformMat4f("u_ModelMat", scene.GetSpotLights()[light].GetModelMat());
			m_Shaders.Lightcube.SetUniformVec3f("u_Color", scene.GetSpotLights()[light].GetColor());
			GLCall(glDrawElements(GL_TRIANGLES, scene.GetData().GetDataGL().GetSpotLightData().IB.GetCount(), GL_UNSIGNED_INT, nullptr));
		}
	}
	scene.GetData().GetDataGL().GetSpotLightData().VA.Unbind();
	scene.GetData().GetDataGL().GetSpotLightData().IB.Unbind();
	// Draw Directional light's cube
	scene.GetData().GetDataGL().GetDirLightData().VA.Bind();
	scene.GetData().GetDataGL().GetDirLightData().IB.Bind();
	GLCall(glDepthFunc(GL_ALWAYS));
	for (unsigned int i = 0; i < scene.GetDirLightList().size(); i++)
	{
		if (scene.GetDirLights()[scene.GetDirLightList()[i]].LightSwitch() && scene.GetDirLights()[scene.GetDirLightList()[i]].ShowCube())
		{
			glm::mat4 modelMat = scene.GetDirLights()[scene.GetDirLightList()[i]].GetModelMat()
								* glm::scale(glm::mat4(1.0f), glm::vec3(glm::length(scene.GetCamera().GetPosition()) / 20.0f));
			m_Shaders.Lightcube.SetUniformMat4f("u_ModelMat", modelMat);
			m_Shaders.Lightcube.SetUniformVec3f("u_Color", scene.GetDirLights()[scene.GetDirLightList()[i]].GetColor());
			GLCall(glDrawElements(GL_TRIANGLES, scene.GetData().GetDataGL().GetDirLightData().IB.GetCount(), GL_UNSIGNED_INT, nullptr));
		}
	}
	GLCall(glDepthFunc(GL_LESS));
	scene.GetData().GetDataGL().GetDirLightData().VA.Unbind();
	scene.GetData().GetDataGL().GetDirLightData().IB.Unbind();
	m_Shaders.Lightcube.Unbind();
	m_Frame.FB.Unbind();
}

void GLRenderer::DrawAreaLightCube(Scene& scene, AL_Type type)
{
	switch (type)
	{
	case RECTANGLE:
		scene.GetData().GetDataGL().GetAreaLightData().RectangleVA.Bind();
		scene.GetData().GetDataGL().GetAreaLightData().RectangleIB.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, scene.GetData().GetDataGL().GetAreaLightData().RectangleIB.GetCount(), GL_UNSIGNED_INT, nullptr));
		scene.GetData().GetDataGL().GetAreaLightData().RectangleVA.Unbind();
		scene.GetData().GetDataGL().GetAreaLightData().RectangleIB.Unbind();
		break;
	case SPHERE:
		scene.GetData().GetDataGL().GetAreaLightData().SphereVA.Bind();
		scene.GetData().GetDataGL().GetAreaLightData().SphereIB.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, scene.GetData().GetDataGL().GetAreaLightData().SphereIB.GetCount(), GL_UNSIGNED_INT, nullptr));
		scene.GetData().GetDataGL().GetAreaLightData().SphereVA.Unbind();
		scene.GetData().GetDataGL().GetAreaLightData().SphereIB.Unbind();
		break;
	case CYLINDER:
		scene.GetData().GetDataGL().GetAreaLightData().CylinderVA.Bind();
		scene.GetData().GetDataGL().GetAreaLightData().CylinderIB.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, scene.GetData().GetDataGL().GetAreaLightData().CylinderIB.GetCount(), GL_UNSIGNED_INT, nullptr));
		scene.GetData().GetDataGL().GetAreaLightData().CylinderVA.Unbind();
		scene.GetData().GetDataGL().GetAreaLightData().CylinderIB.Unbind();
		break;
	case DISK:
		scene.GetData().GetDataGL().GetAreaLightData().DiskVA.Bind();
		scene.GetData().GetDataGL().GetAreaLightData().DiskIB.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, scene.GetData().GetDataGL().GetAreaLightData().DiskIB.GetCount(), GL_UNSIGNED_INT, nullptr));
		scene.GetData().GetDataGL().GetAreaLightData().DiskVA.Unbind();
		scene.GetData().GetDataGL().GetAreaLightData().DiskIB.Unbind();
		break;
	}
}

void GLRenderer::DrawSkybox(Scene& scene)
{
	m_Frame.FB.Bind();
	//GLCall(glDepthFunc(GL_LEQUAL));
	GLCall(glDisable(GL_DEPTH_TEST));
	scene.GetData().GetDataGL().GetSkybox().Shader.Bind();
	scene.GetData().GetDataGL().GetSkybox().Shader.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	// Remove translation from view matrix
	scene.GetData().GetDataGL().GetSkybox().Shader.SetUniformMat4f("u_ViewMat", glm::mat4(glm::mat3(scene.GetCamera().GetViewMat())));
	scene.GetData().GetDataGL().GetSkybox().Shader.SetUniformHandleARB("u_Skybox", scene.GetData().GetDataGL().GetSkybox().Skybox.GetHandle());
	scene.GetData().GetDataGL().GetSkybox().Shader.SetUniform1i("u_UseTex", scene.GetSkybox().Type);
	scene.GetData().GetDataGL().GetSkybox().Shader.SetUniformVec3f("u_Color", scene.GetSkybox().Color);
	scene.GetData().GetDataGL().GetSkybox().VA.Bind();
	scene.GetData().GetDataGL().GetSkybox().IB.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, scene.GetData().GetDataGL().GetSkybox().IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	scene.GetData().GetDataGL().GetSkybox().VA.Unbind();
	scene.GetData().GetDataGL().GetSkybox().IB.Unbind();
	scene.GetData().GetDataGL().GetSkybox().Shader.Unbind();
	GLCall(glEnable(GL_DEPTH_TEST));
	m_Frame.FB.Unbind();
}

void GLRenderer::DrawPointLightShadow(Scene& scene, bool update)
{
	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glCullFace(GL_BACK));
	std::string light;
	scene.GetData().GetDataGL().GetPointLightData().Shader.Bind();
	for (unsigned int i = 0; i < scene.GetPointLightList().size(); i++)
	{
		light = scene.GetPointLightList()[i];
		if (scene.GetPointLights()[light].LightSwitch() && scene.GetPointLights()[light].CastShadow()
			&& (scene.GetPointLights()[light].UpdateShadow() || update))
		{
			scene.GetPointLights()[light].UpdateShadow() = false;
			GLCall(glViewport(0, 0, scene.GetPointLights()[light].GetShadowRes() , scene.GetPointLights()[light].GetShadowRes()));
			scene.GetData().GetDataGL().GetPointLightData().DepthMap[light].Bind();
			GLCall(glClear(GL_DEPTH_BUFFER_BIT));
			for (unsigned int j = 0; j < 6; j++)
			{
				glm::mat4 shadowMat = scene.GetPointLights()[light].GetProjMat() * scene.GetPointLights()[light].GetViewMat(j);
				scene.GetData().GetDataGL().GetPointLightData().Shader.SetUniformMat4f("u_ShadowMat[" + std::to_string(j) + "]", shadowMat);
			}
			scene.GetData().GetDataGL().GetPointLightData().Shader.SetUniformVec3f("u_LightPos", scene.GetPointLights()[light].GetPosition());
			scene.GetData().GetDataGL().GetPointLightData().Shader.SetUniform1f("u_FarPlane", scene.GetPointLights()[light].GetFarPlane());
			// Draw Scene
			std::string model;
			for (unsigned int j = 0; j < scene.GetModelList().size(); j++)
			{
				model = scene.GetModelList()[j];
				scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
				scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();
				// Draw Model
				GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
				scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
				scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
			}
			scene.GetData().GetDataGL().GetPointLightData().DepthMap[light].Unbind();
		}
	}
	scene.GetData().GetDataGL().GetPointLightData().Shader.Unbind();
	GLCall(glDisable(GL_CULL_FACE));
}

void GLRenderer::DrawSpotLightShadow(Scene& scene, bool update)
{
	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glCullFace(GL_BACK));
	std::string light;
	scene.GetData().GetDataGL().GetSpotLightData().Shader.Bind();
	for (unsigned int i = 0; i < scene.GetSpotLightList().size(); i++)
	{
		light = scene.GetSpotLightList()[i];
		if (scene.GetSpotLights()[light].LightSwitch() && scene.GetSpotLights()[light].CastShadow()
			&& (scene.GetSpotLights()[light].UpdateShadow() || update))
		{
			scene.GetSpotLights()[light].UpdateShadow() = false;
			GLCall(glViewport(0, 0, scene.GetSpotLights()[light].GetShadowRes(), scene.GetSpotLights()[light].GetShadowRes()));
			scene.GetData().GetDataGL().GetSpotLightData().DepthMap[light].Bind();
			GLCall(glClear(GL_DEPTH_BUFFER_BIT));
			scene.GetData().GetDataGL().GetSpotLightData().Shader.SetUniformMat4f("u_ProjMat", scene.GetSpotLights()[light].GetProjMat());
			scene.GetData().GetDataGL().GetSpotLightData().Shader.SetUniformMat4f("u_ViewMat", scene.GetSpotLights()[light].GetViewMat());
			// Draw Scene
			std::string model;
			for (unsigned int j = 0; j < scene.GetModelList().size(); j++)
			{
				model = scene.GetModelList()[j];
				scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
				scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();
				// Draw Model
				GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
				scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
				scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
			}
			scene.GetData().GetDataGL().GetSpotLightData().DepthMap[light].Unbind();
		}
	}
	scene.GetData().GetDataGL().GetSpotLightData().Shader.Unbind();
	GLCall(glDisable(GL_CULL_FACE));
}

void GLRenderer::DrawDirLightShadow(Scene& scene)
{
	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glCullFace(GL_BACK));
	std::string light;
	scene.GetData().GetDataGL().GetDirLightData().Shader.Bind();
	for (unsigned int i = 0; i < scene.GetDirLightList().size(); i++)
	{
		light = scene.GetDirLightList()[i];
		if (scene.GetDirLights()[light].LightSwitch() && scene.GetDirLights()[light].CastShadow())
		{
			std::vector<glm::mat4> cameraProj;
			cameraProj.push_back(scene.GetCamera().GetProjMat(0.0f, scene.GetDirLights()[light].GetCSMRatio().x));
			cameraProj.push_back(scene.GetCamera().GetProjMat(scene.GetDirLights()[light].GetCSMRatio().x, scene.GetDirLights()[light].GetCSMRatio().y));
			cameraProj.push_back(scene.GetCamera().GetProjMat(scene.GetDirLights()[light].GetCSMRatio().y, 1.0f));
			scene.GetDirLights()[light].UpdateShadowMat(cameraProj, scene.GetCamera().GetViewMat());
			for (unsigned int j = 0; j < 3; j++)
			{
				unsigned int width = scene.GetData().GetDataGL().GetDirLightData().DepthMap[light][j].GetTexWidth();
				unsigned int height = scene.GetData().GetDataGL().GetDirLightData().DepthMap[light][j].GetTexHeight();
				GLCall(glViewport(0, 0, width, height));
				scene.GetData().GetDataGL().GetDirLightData().DepthMap[light][j].Bind();
				GLCall(glClear(GL_DEPTH_BUFFER_BIT));
				scene.GetData().GetDataGL().GetDirLightData().Shader.SetUniformMat4f("u_ProjMat", scene.GetDirLights()[light].GetProjMat(j));
				scene.GetData().GetDataGL().GetDirLightData().Shader.SetUniformMat4f("u_ViewMat", scene.GetDirLights()[light].GetViewMat(j));
				// Draw Scene
				std::string model;
				for (unsigned int k = 0; k < scene.GetModelList().size(); k++)
				{
					model = scene.GetModelList()[k];
					scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
					scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();
					// Draw Model
					GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
					scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
					scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
				}
				scene.GetData().GetDataGL().GetDirLightData().DepthMap[light][j].Unbind();
			}
		}
	}
	scene.GetData().GetDataGL().GetDirLightData().Shader.Unbind();
	GLCall(glDisable(GL_CULL_FACE));
}

void GLRenderer::DrawDOF(Scene& scene)
{
	GLCall(glDisable(GL_DEPTH_TEST));
	GLCall(glDepthMask(GL_FALSE));
	GLCall(glDisable(GL_BLEND));

	m_Frame.VA.Bind();
	m_Frame.IB.Bind();
	// Generate original coc
	m_Frame.DOF[0].Bind();
	m_Shaders.DOF[0].Bind();
	m_Shaders.DOF[0].SetUniformHandleARB("u_Source", m_Frame.FB.GetHandle());
	m_Shaders.DOF[0].SetUniformHandleARB("u_Depth", m_Frame.GBuffer.GetHandle(2));
	m_Shaders.DOF[0].SetUniform1f("u_Distance", scene.GetCamera().GetFocus().Distance);
	m_Shaders.DOF[0].SetUniform1f("u_Range", scene.GetCamera().GetFocus().Range);
	m_Shaders.DOF[0].SetUniform1f("u_Radius", scene.GetCamera().GetFocus().FocalLength);
	GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	m_Shaders.DOF[0].Unbind();
	m_Frame.DOF[0].Unbind();
	// Downsample the coc
	m_Frame.DOF[2].Bind();
	m_Shaders.DOF[2].Bind();
	glm::vec2 renderRes = core::GetRenderRes() * 0.5f;
	GLCall(glViewport(0, 0, (GLsizei)renderRes.x, (GLsizei)renderRes.y));
	m_Shaders.DOF[2].SetUniformHandleARB("u_Source", m_Frame.DOF[0].GetHandle());
	m_Shaders.DOF[2].SetUniformVec2f("u_TexelSize", 1.0f / glm::vec2(m_Frame.DOF[0].GetTexWidth(), m_Frame.DOF[0].GetTexHeight()));
	GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	m_Shaders.DOF[2].Unbind();
	m_Frame.DOF[2].Unbind();
	// Generate Bokeh using the downsampled img
	m_Frame.DOF[3].Bind();
	m_Shaders.DOF[1].Bind();	
	m_Shaders.DOF[1].SetUniformHandleARB("u_Source", m_Frame.DOF[2].GetHandle());
	m_Shaders.DOF[1].SetUniformVec2f("u_TexelSize", 1.0f / glm::vec2(core::SETTINGS.Width, core::SETTINGS.Height));
	m_Shaders.DOF[1].SetUniform1f("u_Radius", scene.GetCamera().GetFocus().FocalLength);
	GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	m_Shaders.DOF[1].Unbind();
	m_Frame.DOF[3].Unbind();
	// After generate bokeh, perform a gaussian blur on the generated bokeh
	m_Frame.DOF[2].Bind();
	m_Shaders.GaussianBlur.Bind();
	m_Shaders.GaussianBlur.SetUniformHandleARB("u_Source", m_Frame.DOF[3].GetHandle());
	m_Shaders.GaussianBlur.SetUniformVec2f("u_TexelSize", 1.0f / glm::vec2(m_Frame.DOF[3].GetTexWidth(), m_Frame.DOF[3].GetTexHeight()));
	GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	m_Shaders.GaussianBlur.Unbind();
	m_Frame.DOF[2].Unbind();
	// Upsample the blury bokeh to original resolution
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Frame.DOF[2].GetID()));
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Frame.DOF[1].GetID()));
	GLCall(glBlitFramebuffer(0, 0, m_Frame.DOF[2].GetTexWidth(), m_Frame.DOF[2].GetTexHeight(), 0, 0, m_Frame.DOF[1].GetTexWidth(), m_Frame.DOF[1].GetTexHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR));
	renderRes = core::GetRenderRes();
	GLCall(glViewport(0, 0, (GLsizei)renderRes.x, (GLsizei)renderRes.y));
	// Now DOF[0] contains the original img and coc, DOF[1] contains the img applied bokeh, 
	// DOF[2] contains the blury downsampled bokeh img, DOF[3] contains the downsampled bokeh img
	// Combine the bokeh img and the source img
	m_Frame.FB.Bind();
	m_Shaders.DOF[3].Bind();
	m_Shaders.DOF[3].SetUniformHandleARB("u_Source", m_Frame.DOF[0].GetHandle());
	m_Shaders.DOF[3].SetUniformHandleARB("u_DOF", m_Frame.DOF[1].GetHandle());
	GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	m_Shaders.DOF[3].Unbind();
	m_Frame.FB.Unbind();
	m_Frame.VA.Unbind();
	m_Frame.IB.Unbind();
	GLCall(glEnable(GL_BLEND));
	GLCall(glDepthMask(GL_TRUE));
	GLCall(glEnable(GL_DEPTH_TEST));
}

void GLRenderer::DrawBloom(Scene& scene)
{
	GLCall(glDepthMask(GL_FALSE));
	// Copy current screen buffer content to a new fb
	int width = m_Frame.FB.GetTexWidth();
	int height = m_Frame.FB.GetTexHeight();
	m_Frame.Bloom[0].Bind();
	Clear();
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Frame.FB.GetID()));
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Frame.Bloom[0].GetID()));
	GLCall(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
	m_Frame.Bloom[0].Unbind();
	m_Frame.Bloom[6].Bind();
	Clear();
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Frame.FB.GetID()));
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Frame.Bloom[6].GetID()));
	GLCall(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
	m_Frame.Bloom[6].Unbind();
	// Downsample
	GLCall(glDisable(GL_BLEND));
	m_Shaders.Bloom[0].Bind();
	m_Frame.VA.Bind();
	m_Frame.IB.Bind();
	GLCall(glDisable(GL_DEPTH_TEST));
	for (unsigned int i = 0; i < 5; i++)
	{
		if (m_Frame.Bloom[i].GetID() == 0)
		{
			break;
		}
		int width = m_Frame.Bloom[i + 1].GetTexWidth();
		int height = m_Frame.Bloom[i + 1].GetTexHeight();
		GLCall(glViewport(0, 0, width, height));
		int srcWidth = m_Frame.Bloom[i].GetTexWidth();
		int srcHeight = m_Frame.Bloom[i].GetTexHeight();
		m_Frame.Bloom[i + 1].Bind();
		Clear();
		m_Frame.Bloom[i].BindTex(0);
		m_Shaders.Bloom[0].SetUniform1i("u_SrcTex", 0);
		m_Shaders.Bloom[0].SetUniformVec2f("u_SrcTexelSize", 1.0f / glm::vec2(srcWidth, srcHeight));
		GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
		m_Frame.Bloom[i].UnbindTex();
		m_Frame.Bloom[i + 1].Unbind();
	}
	m_Shaders.Bloom[0].Unbind();
	GLCall(glEnable(GL_BLEND));
	// Upsample
	m_Shaders.Bloom[1].Bind();
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_ONE, GL_ONE));
	GLCall(glBlendEquation(GL_FUNC_ADD));
	for (unsigned int i = 5; i > 0; i--)
	{
		if (m_Frame.Bloom[i].GetID() == 0)
		{
			continue;
		}
		int width = m_Frame.Bloom[i - 1].GetTexWidth();
		int height = m_Frame.Bloom[i - 1].GetTexHeight();
		GLCall(glViewport(0, 0, width, height));
		m_Frame.Bloom[i - 1].Bind();
		Clear();
		m_Frame.Bloom[i].BindTex(0);
		m_Shaders.Bloom[1].SetUniform1i("u_SrcTex", 0);
		m_Shaders.Bloom[1].SetUniform1f("u_FilterRadius", scene.GetCamera().GetBloom().FilterRadius);
		GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
		m_Frame.Bloom[i].UnbindTex();
		m_Frame.Bloom[i - 1].Unbind();
	}
	m_Shaders.Bloom[1].Unbind();
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	// Blend result
	m_Shaders.Bloom[2].Bind();
	width = m_Frame.FB.GetTexWidth();
	height = m_Frame.FB.GetTexHeight();
	GLCall(glViewport(0, 0, width, height));
	m_Frame.FB.Bind();
	Clear();
	m_Frame.Bloom[0].BindTex();
	m_Frame.Bloom[6].BindTex(1);
	m_Shaders.Bloom[2].SetUniform1i("u_ScreenTex", 1);
	m_Shaders.Bloom[2].SetUniform1i("u_BloomTex", 0);
	m_Shaders.Bloom[2].SetUniform1f("u_BloomStrength", scene.GetCamera().GetBloom().Strength);
	GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	m_Frame.Bloom[0].UnbindTex();
	m_Frame.Bloom[6].UnbindTex();
	m_Frame.FB.Unbind();
	m_Shaders.Bloom[2].Unbind();
	m_Frame.VA.Unbind();
	m_Frame.IB.Unbind();
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthMask(GL_TRUE));
}

void GLRenderer::DrawSSAO(Scene& scene)
{
	GLCall(glDisable(GL_DEPTH_TEST));
	GLCall(glDisable(GL_BLEND));
	m_Frame.VA.Bind();
	m_Frame.IB.Bind();
	GLCall(glViewport(0, 0, m_Frame.SSAO[0].GetTexWidth(), m_Frame.SSAO[0].GetTexHeight()));
	// Generate SSAO texture
	m_Frame.SSAO[0].Bind();
	Clear();
	m_Shaders.SSAO[0].Bind();
	m_Shaders.SSAO[0].SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.SSAO[0].SetUniformHandleARB("u_Position", m_Frame.GBuffer.GetHandle(0));
	m_Shaders.SSAO[0].SetUniformHandleARB("u_Normal", m_Frame.GBuffer.GetHandle(1));
	glm::vec2 noiseSize = glm::vec2(scene.GetData().GetDataGL().GetVFXData().SSAONoiseTex.GetWidth(),
									scene.GetData().GetDataGL().GetVFXData().SSAONoiseTex.GetHeight());
	glm::vec2 noiseScale = glm::vec2(m_Frame.SSAO[0].GetTexWidth() / noiseSize.x, m_Frame.SSAO[0].GetTexHeight() / noiseSize.y);
	m_Shaders.SSAO[0].SetUniformVec2f("u_NoiseScale", noiseScale);
	m_Shaders.SSAO[0].SetUniform1i("u_KernelSize", scene.GetVFX().SSAOKernelSize);
	m_Shaders.SSAO[0].SetUniform1f("u_Radius", scene.GetVFX().SSAORadius);
	m_Shaders.SSAO[0].SetUniform1f("u_Bias", scene.GetVFX().SSAOBias);
	GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	m_Shaders.SSAO[0].Unbind();
	m_Frame.SSAO[0].Unbind();
	// Blur the SSAO texture
	m_Frame.SSAO[1].Bind();
	Clear();
	m_Shaders.SSAO[1].Bind();
	m_Shaders.SSAO[1].SetUniformHandleARB("u_SSAO", m_Frame.SSAO[0].GetHandle());
	GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	m_Shaders.SSAO[1].Unbind();
	m_Frame.SSAO[1].Unbind();
	m_Frame.VA.Unbind();
	m_Frame.IB.Unbind();
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_BLEND));
}

void GLRenderer::DrawFXAA(Scene& scene)
{
	GLCall(glDisable(GL_DEPTH_TEST));
	GLCall(glDepthMask(GL_FALSE));
	GLCall(glDisable(GL_BLEND));
	m_Frame.FXAA.Bind();
	m_Shaders.FXAA.Bind();
	m_Shaders.FXAA.SetUniformHandleARB("u_Source", m_Frame.FB.GetHandle()); 
	m_Shaders.FXAA.SetUniformVec2f("u_InverseScreenSize", 1.0f / core::GetRenderRes());
	m_Frame.VA.Bind();
	m_Frame.IB.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	m_Frame.VA.Unbind();
	m_Frame.IB.Unbind();
	m_Shaders.FXAA.Unbind();
	m_Frame.FXAA.Unbind();
	GLCall(glEnable(GL_BLEND));
	GLCall(glDepthMask(GL_TRUE));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Frame.FXAA.GetID()));
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Frame.FB.GetID()));
	GLCall(glBlitFramebuffer(0, 0, m_Frame.FXAA.GetTexWidth(), m_Frame.FXAA.GetTexHeight(), 0, 0, m_Frame.FB.GetTexWidth(), m_Frame.FB.GetTexHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR));
}

void GLRenderer::DrawAutoExposure(Scene& scene)
{
	// First reset histogram
	scene.GetData().GetDataGL().GetVFXData().ExpoHistogram.assign(scene.GetData().GetDataGL().GetVFXData().ExpoHistogram.size(), 0);
	scene.GetData().GetDataGL().GetVFXData().ExpoHistoDB.SetData(&scene.GetData().GetDataGL().GetVFXData().ExpoHistogram[0], 1024, GL_DYNAMIC_COPY);
	// Then transform the original frame into histogram
	m_Shaders.Exposure[0].Bind();
	GLCall(glBindImageTexture(0, m_Frame.FB.GetTexID(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F));
	scene.GetData().GetDataGL().GetVFXData().ExpoHistoDB.Bind(1);
	GLCall(glDispatchCompute(ceil(m_Frame.FB.GetTexWidth() / 32.0), ceil(m_Frame.FB.GetTexHeight() / 32.0), 1));
	GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
	scene.GetData().GetDataGL().GetVFXData().ExpoHistoDB.Unbind();
	m_Shaders.Exposure[0].Unbind();
	// Calculate the average luminance
	m_Shaders.Exposure[1].Bind();
	scene.GetData().GetDataGL().GetVFXData().ExpoHistoDB.Bind(0);
	scene.GetData().GetDataGL().GetVFXData().ExpoAvgDB.Bind(1);
	glm::vec2 renderRes = core::GetRenderRes();
	m_Shaders.Exposure[1].SetUniform1f("u_TimeDelta", core::TIME_DELTA);
	m_Shaders.Exposure[1].SetUniform1i("u_PixelNum", (int)(renderRes.x * renderRes.y));
	GLCall(glDispatchCompute(1, 1, 1));
	GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
	m_Shaders.Exposure[1].Unbind();
	scene.GetData().GetDataGL().GetVFXData().ExpoAvgDB.GetData(&scene.GetData().GetDataGL().GetVFXData().ExpoAvg, 0, sizeof(float));
	float exposure = 1.0f / (9.6f * scene.GetData().GetDataGL().GetVFXData().ExpoAvg + 0.0001f);
	scene.GetCamera().SetExposure(exposure);
}

void GLRenderer::DrawToneMapping(Scene& scene)
{
	GLCall(glDisable(GL_DEPTH_TEST));
	GLCall(glViewport(0, 0, core::GetRenderRes().x, core::GetRenderRes().y));
	m_Frame.Screen.Bind();
	m_Shaders.Screen.Bind();
	m_Shaders.Screen.SetUniformHandleARB("u_ScreenTex", m_Frame.FB.GetHandle());
	m_Shaders.Screen.SetUniform1f("u_Gamma", core::SETTINGS.Gamma);
	m_Shaders.Screen.SetUniform1f("u_Exposure", scene.GetCamera().GetExposure().Strength);
	m_Shaders.Screen.SetUniform1i("u_TonemapCurve", core::SETTINGS.TonemapCurve);
	m_Frame.VA.Bind();
	m_Frame.IB.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	m_Shaders.Screen.Unbind();
	m_Frame.Screen.Unbind();
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Frame.Screen.GetID()));
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Frame.FB.GetID()));
	int width = m_Frame.Screen.GetTexWidth();
	int height = m_Frame.Screen.GetTexHeight();
	GLCall(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR));
}

bool GLRenderer::SaveScreenShot(Scene& scene)
{
	// Make the BYTE array, factor of 3 because it's RBG.
	BYTE* pixels = new BYTE[3 * core::SETTINGS.Width * core::SETTINGS.Height];
	glReadPixels(0, 0, core::SETTINGS.Width, core::SETTINGS.Height, GL_BGR, GL_UNSIGNED_BYTE, pixels);
	// Generate screenshot file name
	SYSTEMTIME time;
	GetLocalTime(&time);
	std::string filepath = SCREENSHOT_FILEPATH
							+ scene.GetName() + "_"
							+ std::to_string(time.wYear) 
							+ std::to_string(time.wMonth)
							+ std::to_string(time.wDay)
					  + "_" + std::to_string(time.wHour)
							+ std::to_string(time.wMinute)
							+ std::to_string(time.wSecond)
							+ ".png";
	// Convert to FreeImage format & save to file
	FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, core::SETTINGS.Width, core::SETTINGS.Height, 3 * core::SETTINGS.Width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
	if (!FreeImage_Save(FIF_PNG, image, filepath.c_str(), PNG_Z_BEST_SPEED))
	{
		core::ShowWarningMsg("Unknow error! Couldn't save screenshot!");
		return false;
	}
	core::ShowWarningMsg("Screenshot saved at: " + filepath);
	FreeImage_Unload(image);
	delete[] pixels;
	return true;
}

void GLRenderer::ChangeResolution()
{
	int width = (int)(core::SETTINGS.Width * core::SETTINGS.Resolution);
	int height = (int)(core::SETTINGS.Height * core::SETTINGS.Resolution);

	m_Frame.FB.Init(FBType::FRAME, width, height);
	m_Frame.Screen.Init(FBType::FRAME, width, height);
	m_Frame.FXAA.Init(FBType::FRAME);
	m_Frame.GBuffer.Init(FBType::G_BUFFER, width, height);
	m_Frame.SSAO[0].Init(FBType::SSAO, width, height);
	m_Frame.SSAO[1].Init(FBType::SSAO, width, height);
	m_Frame.OIT.Init(FBType::OIT, width, height);
	m_Frame.DOF[0].Init(FBType::FRAME, width, height);
	m_Frame.DOF[1].Init(FBType::FRAME, width, height);
	glm::vec2 renderRes = core::GetRenderRes() * 0.5f;
	m_Frame.DOF[2].Init(FBType::FRAME, (int)renderRes.x, (int)renderRes.y);
	m_Frame.DOF[3].Init(FBType::FRAME, (int)renderRes.x, (int)renderRes.y);
	// Initialize framebuffers used for bloom effect
	for (unsigned int i = 0; i < 7; i++)
	{
		m_Frame.Bloom[i].~GLFrameBuffer();
	}
	int bloomWidth = (int)(core::SETTINGS.Width * core::SETTINGS.Resolution);
	int bloomHeight = (int)(core::SETTINGS.Height * core::SETTINGS.Resolution);
	m_Frame.Bloom[6].Init(FBType::FRAME, bloomWidth, bloomHeight);
	for (unsigned int i = 0; i < 6; i++)
	{
		if (bloomWidth > 6 && bloomHeight > 6)
		{
			m_Frame.Bloom[i].Init(FRAME, bloomWidth, bloomHeight);
			bloomWidth /= 2;
			bloomHeight /= 2;
		}
		else
		{
			break;
		}
	}
}

bool GLRenderer::ChangePostProcess()
{
	switch (core::SETTINGS.PP)
	{
	case 0:
		if (!m_Shaders.PP.Init(SHADER_OPENGL_PP_NONE))
		{
			return false;
		}
		break;
	case 1:
		if (!m_Shaders.PP.Init(SHADER_OPENGL_PP_INVERSE))
		{
			return false;
		}
		break;
	case 2:
		if (!m_Shaders.PP.Init(SHADER_OPENGL_PP_BLUR))
		{
			return false;
		}
		break;
	case 3:
		if (!m_Shaders.PP.Init(SHADER_OPENGL_PP_GRAYSCALE))
		{
			return false;
		}
		break;
	case 4:
		if (!m_Shaders.PP.Init(SHADER_OPENGL_PP_EDGE))
		{
			return false;
		}
		break;
	}
	return true;
}