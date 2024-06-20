#include "GLRenderer.h"

GLRenderer::GLRenderer()
{
}

GLRenderer::~GLRenderer()
{
}

void GLRenderer::Init(Scene& scene)
{
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_DEPTH_TEST));
	//GLCall(glEnable(GL_CULL_FACE));
	//GLCall(glCullFace(GL_BACK));
	GLCall(glLineWidth(0.4f));
	GLCall(glPointSize(1.5f));
	// Initialize shaders
	m_Shaders.GBuffer.Init(SHADER_OPENGL_GBUFFER);
	m_Shaders.Normal.Init(SHADER_OPENGL_NORMAL);
	m_Shaders.Lightcube.Init(SHADER_OPENGL_LIGHTCUBE);
	m_Shaders.Bloom[0].Init(SHADER_OPENGL_BLOOM_DOWNSAMPLE); 
	m_Shaders.Bloom[1].Init(SHADER_OPENGL_BLOOM_UPSAMPLE);
	m_Shaders.Bloom[2].Init(SHADER_OPENGL_BLOOM_BLEND);
	// Shaders used for ssao
	{
		m_Shaders.SSAO[0].Init(SHADER_OPENGL_SSAO_GEN);
		m_Shaders.SSAO[0].Bind();
		for (unsigned int i = 0; i < 64; i++)
		{
			m_Shaders.SSAO[0].SetUniformVec3f("u_Samples[" + std::to_string(i) + "]", scene.GetData().GetDataGL().GetVFXData().SSAOSamples[i]);
		}
		m_Shaders.SSAO[0].SetUniformHandleARB("u_NoiseTex", scene.GetData().GetDataGL().GetVFXData().SSAONoiseTex.GetHandle());
		m_Shaders.SSAO[0].Unbind();
		m_Shaders.SSAO[1].Init(SHADER_OPENGL_SSAO_BLUR);
	}
	ChangePostProcess();
	// Initialize frame buffers
	m_Frame.FBMsaa.Init(FBType::MSAA);
	if (!m_Frame.FBMsaa.IsInitialized())
	{
		core::ShowWarningMsg("Failed to initialize OpenGL renderer!");
	}
	m_Frame.FB.Init(FBType::FRAME);
	m_Frame.GBuffer.Init(FBType::G_BUFFER);
	// Initialize framebuffers used for ssao
	m_Frame.SSAO[0].Init(FBType::SSAO); 
	m_Frame.SSAO[1].Init(FBType::SSAO);
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
		m_Frame.VB.GenVertexBuffer(position, sizeof(position));
		m_Frame.IB.GenIndexBuffer(indices, 6);
		GLVertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_Frame.VA.AddBuffer(m_Frame.VB, layout);
	}
	else
	{
		core::ShowWarningMsg("Failed to initialize OpenGL renderer!");
	}	
}

void GLRenderer::UpdateModelMat(Scene& scene)
{
	std::string model;
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].InstanceVB.UpdateVertexBuffer(
			&scene.GetModels()[model].GetModelMats()[0],
			(unsigned int)scene.GetModels()[model].GetModelMats().size() * sizeof(glm::mat4)
		);
	}
}

void GLRenderer::Clear()
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
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
		DrawPointLightShadow(scene);
		DrawSpotLightShadow(scene);
		DrawDirLightShadow(scene);
	}
	// MSAA
	if ((int)core::SETTINGS.AA >= 1 && (int)core::SETTINGS.AA <= 4)
	{
		m_Frame.FBMsaa.Bind();
	}
	else
	{
		m_Frame.FB.Bind();
	}
	// Draw your content inside this scope
	Clear();
	glm::vec2 renderRes = core::GetRenderResolution();
	GLCall(glViewport(0, 0, (GLsizei)renderRes.x, (GLsizei)renderRes.y));
	switch (core::SETTINGS.DrawMode)
	{
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
	}
	// Draw Light Cube
	DrawLightCube(scene);
	// Draw normals
	if (core::SETTINGS.ShowNormal)
	{
		DrawNormal(scene);
	}
	// Draw Skybox
	DrawSkybox(scene);
	// Draw your content inside this scope
	if ((int)core::SETTINGS.AA >= 1 && (int)core::SETTINGS.AA <= 4)
	{
		GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Frame.FBMsaa.GetID()));
		GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Frame.FB.GetID()));
		int width = (int)(core::SETTINGS.Width * core::SETTINGS.Resolution);
		int height = (int)(core::SETTINGS.Height * core::SETTINGS.Resolution);
		GLCall(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
	}
	m_Frame.FB.Unbind();
	if (scene.GetVFX().Bloom && (core::SETTINGS.DrawMode == BLANK || core::SETTINGS.DrawMode == DEFAULT))
	{
		DrawBloom(scene);
	}
	Clear();
	// Draw frame buffer's content on the screen;
	GLCall(glViewport(0, 0, core::SETTINGS.Width, core::SETTINGS.Height));
	GLCall(glDisable(GL_DEPTH_TEST));
	m_Shaders.Screen.Bind();
	m_Frame.FB.BindTex();
	m_Shaders.Screen.SetUniform1i("u_ScreenTex", 0);
	m_Shaders.Screen.SetUniform1f("u_Gamma", core::SETTINGS.Gamma);
	m_Frame.VA.Bind();
	m_Frame.IB.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	m_Frame.VA.Unbind();
	m_Frame.IB.Unbind();
	m_Shaders.Screen.Unbind();
	m_Frame.FB.UnbindTex();
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
	m_Frame.GBuffer.Unbind();
}

void GLRenderer::DrawBlank(Scene& scene)
{
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
					scene.GetData().GetDataGL().GetShader().SetUniformVec3f(lightName + "Bias", scene.GetDirLights()[light].GetBias());
				}
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
}

void GLRenderer::DrawWireFrame(Scene& scene)
{
	std::string model;
	scene.GetData().GetDataGL().GetShader().Bind();
	scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	scene.GetData().GetDataGL().GetShader().SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	glm::vec2 renderRes = core::GetRenderResolution();
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
}

void GLRenderer::DrawPointCloud(Scene& scene)
{
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
}

void GLRenderer::DrawDepth(Scene& scene)
{
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
}

void GLRenderer::DrawUVSet(Scene& scene)
{
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
}

void GLRenderer::DrawNormal(Scene& scene)
{
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
}

void GLRenderer::DrawLightCube(Scene& scene)
{
	m_Shaders.Lightcube.Bind();
	glm::vec2 renderRes = core::GetRenderResolution();
	m_Shaders.Lightcube.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.Lightcube.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	m_Shaders.Lightcube.SetUniformMat4f("u_ViewPortMat", core::GetViewPortMatrix((unsigned int)renderRes.x, (unsigned int)renderRes.y));
	m_Shaders.Lightcube.SetUniformVec2f("u_RenderRes", renderRes);
	// Draw Point light's cube
	scene.GetData().GetDataGL().GetPointLightData().VA.Bind();
	scene.GetData().GetDataGL().GetPointLightData().IB.Bind();
	for (unsigned int i = 0; i < scene.GetPointLightList().size(); i++)
	{
		if (scene.GetPointLights()[scene.GetPointLightList()[i]].LightSwitch() && scene.GetPointLights()[scene.GetPointLightList()[i]].ShowCube())
		{
			m_Shaders.Lightcube.SetUniformMat4f("u_ModelMat", scene.GetPointLights()[scene.GetPointLightList()[i]].GetModelMat());
			m_Shaders.Lightcube.SetUniformVec3f("u_Color", scene.GetPointLights()[scene.GetPointLightList()[i]].GetColor());
			m_Shaders.Lightcube.SetUniform1f("u_Intensity", scene.GetPointLights()[scene.GetPointLightList()[i]].GetIntensity());
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
		if (scene.GetSpotLights()[scene.GetSpotLightList()[i]].LightSwitch() && scene.GetSpotLights()[scene.GetSpotLightList()[i]].ShowCube())
		{
			m_Shaders.Lightcube.SetUniformMat4f("u_ModelMat", scene.GetSpotLights()[scene.GetSpotLightList()[i]].GetModelMat());
			m_Shaders.Lightcube.SetUniformVec3f("u_Color", scene.GetSpotLights()[scene.GetSpotLightList()[i]].GetColor());
			m_Shaders.Lightcube.SetUniform1f("u_Intensity", scene.GetSpotLights()[scene.GetSpotLightList()[i]].GetIntensity());
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
			m_Shaders.Lightcube.SetUniform1f("u_Intensity", scene.GetDirLights()[scene.GetDirLightList()[i]].GetIntensity());
			GLCall(glDrawElements(GL_TRIANGLES, scene.GetData().GetDataGL().GetDirLightData().IB.GetCount(), GL_UNSIGNED_INT, nullptr));
		}
	}
	GLCall(glDepthFunc(GL_LESS));
	scene.GetData().GetDataGL().GetDirLightData().VA.Unbind();
	scene.GetData().GetDataGL().GetDirLightData().IB.Unbind();
	m_Shaders.Lightcube.Unbind();
}

void GLRenderer::DrawSkybox(Scene& scene)
{
	GLCall(glDepthFunc(GL_LEQUAL));
	scene.GetData().GetDataGL().GetSkybox().Shader.Bind();
	scene.GetData().GetDataGL().GetSkybox().Shader.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	// Remove translation from view matrix
	scene.GetData().GetDataGL().GetSkybox().Shader.SetUniformMat4f("u_ViewMat", glm::mat4(glm::mat3(scene.GetCamera().GetViewMat())));
	scene.GetData().GetDataGL().GetSkybox().Shader.SetUniform1i("u_UseTex", scene.GetSkybox().Type);
	scene.GetData().GetDataGL().GetSkybox().Shader.SetUniformVec3f("u_Color", scene.GetSkybox().Color);
	scene.GetData().GetDataGL().GetSkybox().VA.Bind();
	scene.GetData().GetDataGL().GetSkybox().IB.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, scene.GetData().GetDataGL().GetSkybox().IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	scene.GetData().GetDataGL().GetSkybox().VA.Unbind();
	scene.GetData().GetDataGL().GetSkybox().IB.Unbind();
	scene.GetData().GetDataGL().GetSkybox().Shader.Unbind();
	GLCall(glDepthFunc(GL_LESS)); // set depth function back to default
}

void GLRenderer::DrawPointLightShadow(Scene& scene)
{
	std::string light;
	scene.GetData().GetDataGL().GetPointLightData().Shader.Bind();
	for (unsigned int i = 0; i < scene.GetPointLightList().size(); i++)
	{
		light = scene.GetPointLightList()[i];
		// If light's shadow can't be seen, then skip
		{
			// TODO
		}
		if (scene.GetPointLights()[light].LightSwitch() && scene.GetPointLights()[light].CastShadow())
		{
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
}

void GLRenderer::DrawSpotLightShadow(Scene& scene)
{
	std::string light;
	scene.GetData().GetDataGL().GetSpotLightData().Shader.Bind();
	for (unsigned int i = 0; i < scene.GetSpotLightList().size(); i++)
	{
		light = scene.GetSpotLightList()[i];
		// If light's shadow can't be seen, then skip
		{
			// TODO
		}
		if (scene.GetSpotLights()[light].LightSwitch() && scene.GetSpotLights()[light].CastShadow())
		{
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
}

void GLRenderer::DrawDirLightShadow(Scene& scene)
{
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
}

void GLRenderer::DrawBloom(Scene& scene)
{
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
		m_Shaders.Bloom[1].SetUniform1f("u_FilterRadius", scene.GetVFX().BloomFilterRadius);
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
	m_Shaders.Bloom[2].SetUniform1f("u_BloomStrength", scene.GetVFX().BloomStrength);
	GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	m_Frame.Bloom[0].UnbindTex();
	m_Frame.Bloom[6].UnbindTex();
	m_Frame.FB.Unbind();
	m_Shaders.Bloom[2].Unbind();
	m_Frame.VA.Unbind();
	m_Frame.IB.Unbind();
	GLCall(glEnable(GL_DEPTH_TEST));
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

bool GLRenderer::SaveScreenShot()
{
	// Make the BYTE array, factor of 3 because it's RBG.
	BYTE* pixels = new BYTE[3 * core::SETTINGS.Width * core::SETTINGS.Height];
	glReadPixels(0, 0, core::SETTINGS.Width, core::SETTINGS.Height, GL_BGR, GL_UNSIGNED_BYTE, pixels);
	// Generate screenshot file name
	SYSTEMTIME time;
	GetLocalTime(&time);
	std::string filepath = SCREENSHOT_FILEPATH 
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
	m_Frame.FB.ChangeResolution();
	m_Frame.FBMsaa.ChangeResolution();
	m_Frame.GBuffer.Init(FBType::G_BUFFER);
	m_Frame.SSAO[0].Init(FBType::SSAO);
	m_Frame.SSAO[1].Init(FBType::SSAO);
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

void GLRenderer::ChangeMSAA()
{
	m_Frame.FBMsaa.ChangeMSAA();
}

void GLRenderer::ChangePostProcess()
{
	m_Shaders.Screen.Bind();
	switch (core::SETTINGS.PP)
	{
	case NO_PP:
		m_Shaders.Screen.Init((std::string)SHADER_OPENGL + "post_process/DEFAULT.glsl");
		break;
	case INVERSE:
		m_Shaders.Screen.Init((std::string)SHADER_OPENGL + "post_process/INVERSE.glsl");
		break;
	case BLUR:
		m_Shaders.Screen.Init((std::string)SHADER_OPENGL + "post_process/BLUR.glsl");
		break;
	case GRAY_SCALE:
		m_Shaders.Screen.Init((std::string)SHADER_OPENGL + "post_process/GRAYSCALE.glsl");
		break;
	case EDGE:
		m_Shaders.Screen.Init((std::string)SHADER_OPENGL + "post_process/EDGE.glsl");
		break;
	default:
		m_Shaders.Screen.Init((std::string)SHADER_OPENGL + "post_process/DEFAULT.glsl");
		break;
	}
	m_Shaders.Screen.Unbind();
}