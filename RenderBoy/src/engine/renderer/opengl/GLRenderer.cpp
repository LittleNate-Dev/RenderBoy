#include "GLRenderer.h"

GLRenderer::GLRenderer()
{
}

GLRenderer::~GLRenderer()
{
}

void GLRenderer::Init()
{
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glLineWidth(0.4f));
	GLCall(glPointSize(1.5f));
	// Initialize shaders
	m_Shaders.Wireframe.Init((std::string)SHADER_OPENGL + "WIREFRAME.glsl");
	m_Shaders.Pointcloud.Init((std::string)SHADER_OPENGL + "POINTCLOUD.glsl");
	m_Shaders.Depth.Init((std::string)SHADER_OPENGL + "DEPTH.glsl");
	m_Shaders.Normal.Init((std::string)SHADER_OPENGL + "NORMAL.glsl");
	m_Shaders.Lightcube.Init((std::string)SHADER_OPENGL + "LIGHTCUBE.glsl");
	m_Shaders.UVset.Init((std::string)SHADER_OPENGL + "UVSET.glsl");
	// Initialize uv checker map texture
	m_CheckerMap.GenTexture(UV_MAP_FILEPATH);
	m_Shaders.UVset.Bind();
	m_Shaders.UVset.SetUniformHandleARB("u_CheckerMap", m_CheckerMap.GetHandle());
	m_Shaders.UVset.Unbind();
	ChangePostProcess();
	// Initialize frame buffers
	m_Frame.FBMsaa.Init(FBType::MSAA);
	if (!m_Frame.FBMsaa.IsInitialized())
	{
		rbcore::ShowWarningMsg("Failed to initialize OpenGL renderer!");
	}
	m_Frame.FB.Init(FBType::FRAME);
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
		rbcore::ShowWarningMsg("Failed to initialize OpenGL renderer!");
	}	
}

void GLRenderer::Clear()
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void GLRenderer::Draw(Scene& scene)
{
	// MSAA
	if ((int)rbcore::SETTINGS.AA >= 1 && (int)rbcore::SETTINGS.AA <= 4)
	{
		m_Frame.FBMsaa.Bind();
	}
	else
	{
		m_Frame.FB.Bind();
	}
	// Draw your content inside this scope
	Clear();
	glm::vec2 renderRes = rbcore::GetRenderResolution();
	GLCall(glViewport(0, 0, (GLsizei)renderRes.x, (GLsizei)renderRes.y));
	switch (rbcore::SETTINGS.DrawMode)
	{
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
	if (rbcore::SETTINGS.ShowNormal)
	{
		DrawNormal(scene);
	}
	// Draw your content inside this scope
	if ((int)rbcore::SETTINGS.AA >= 1 && (int)rbcore::SETTINGS.AA <= 4)
	{
		GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Frame.FBMsaa.GetID()));
		GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Frame.FB.GetID()));
		int width = (int)(rbcore::SETTINGS.Width * rbcore::SETTINGS.Resolution);
		int height = (int)(rbcore::SETTINGS.Height * rbcore::SETTINGS.Resolution);
		GLCall(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
	}
	m_Frame.FB.Unbind();
	Clear();
	GLCall(glViewport(0, 0, rbcore::SETTINGS.Width, rbcore::SETTINGS.Height));
	// Draw frame buffer's content on the screen;
	GLCall(glDisable(GL_DEPTH_TEST));
	m_Shaders.Screen.Bind();
	m_Frame.FB.BindTex();
	m_Shaders.Screen.SetUniform1i("u_ScreenTex", 0);
	m_Shaders.Screen.SetUniform1f("u_Gamma", rbcore::SETTINGS.Gamma);
	m_Frame.VA.Bind();
	m_Frame.IB.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_Frame.IB.GetCount(), GL_UNSIGNED_INT, nullptr));
	m_Frame.VA.Unbind();
	m_Frame.IB.Unbind();
	m_Shaders.Screen.Unbind();
	m_Frame.FB.UnbindTex();
	GLCall(glEnable(GL_DEPTH_TEST));
}

void GLRenderer::DrawWireFrame(Scene& scene)
{
	std::string model;
	m_Shaders.Wireframe.Bind();
	m_Shaders.Wireframe.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.Wireframe.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	glm::vec2 renderRes = rbcore::GetRenderResolution();
	m_Shaders.Wireframe.SetUniformMat4f("u_ViewPortMat", rbcore::GetViewPortMatrix((unsigned int)renderRes.x, (unsigned int)renderRes.y));
	m_Shaders.Wireframe.SetUniformVec2f("u_RenderRes", renderRes);
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].InstanceVB.UpdateVertexBuffer(
			&scene.GetModels()[model].GetModelMats()[0], 
			(unsigned int)scene.GetModels()[model].GetModelMats().size() * sizeof(glm::mat4)
		);
		scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();
		// Draw Model
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
	}
	m_Shaders.Wireframe.Unbind();
}

void GLRenderer::DrawPointCloud(Scene& scene)
{
	std::string model;
	m_Shaders.Pointcloud.Bind();
	m_Shaders.Pointcloud.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.Pointcloud.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].InstanceVB.UpdateVertexBuffer(
			&scene.GetModels()[model].GetModelMats()[0],
			(unsigned int)scene.GetModels()[model].GetModelMats().size() * sizeof(glm::mat4)
		);
		scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();
		GLCall(glDrawElementsInstanced(GL_POINTS, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
	}
	m_Shaders.Pointcloud.Unbind();
}

void GLRenderer::DrawDepth(Scene& scene)
{
	std::string model;
	m_Shaders.Depth.Bind();
	m_Shaders.Depth.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.Depth.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	m_Shaders.Depth.SetUniform1f("u_NearPlane", scene.GetCamera().GetNearPlane());
	m_Shaders.Depth.SetUniform1f("u_FarPlane", scene.GetCamera().GetFarPlane());
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].InstanceVB.UpdateVertexBuffer(
			&scene.GetModels()[model].GetModelMats()[0],
			(unsigned int)scene.GetModels()[model].GetModelMats().size() * sizeof(glm::mat4)
		);
		scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
	}
	m_Shaders.Depth.Unbind();
}

void GLRenderer::DrawUVSet(Scene& scene)
{
	std::string model;
	m_Shaders.UVset.Bind();
	m_Shaders.UVset.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.UVset.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].InstanceVB.UpdateVertexBuffer(
			&scene.GetModels()[model].GetModelMats()[0],
			(unsigned int)scene.GetModels()[model].GetModelMats().size() * sizeof(glm::mat4)
		);
		scene.GetData().GetDataGL().GetModelData()[model].VA.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Bind();
		// Draw Model
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].IB.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].VA.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].IB.Unbind();
	}
	m_Shaders.UVset.Unbind();
}

void GLRenderer::DrawNormal(Scene& scene)
{
	std::string model;
	m_Shaders.Normal.Bind();
	m_Shaders.Normal.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.Normal.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	m_Shaders.Normal.SetUniform1f("u_Magnitude", rbcore::NORMAL_MAGNITUDE);
	m_Shaders.Normal.SetUniformVec3f("u_Color", rbcore::NORMAL_COLOR);
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].InstanceVB.UpdateVertexBuffer(
			&scene.GetModels()[model].GetModelMats()[0],
			(unsigned int)scene.GetModels()[model].GetModelMats().size() * sizeof(glm::mat4)
		);
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
	glm::vec2 renderRes = rbcore::GetRenderResolution();
	m_Shaders.Lightcube.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.Lightcube.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	m_Shaders.Lightcube.SetUniformMat4f("u_ViewPortMat", rbcore::GetViewPortMatrix((unsigned int)renderRes.x, (unsigned int)renderRes.y));
	m_Shaders.Lightcube.SetUniformVec2f("u_RenderRes", renderRes);
	// Draw Point light's cube
	scene.GetData().GetDataGL().GetLightCube(POINT_LIGHT).VA.Bind();
	scene.GetData().GetDataGL().GetLightCube(POINT_LIGHT).IB.Bind();
	for (unsigned int i = 0; i < scene.GetPointLightList().size(); i++)
	{
		if (scene.GetPointLights()[scene.GetPointLightList()[i]].ShowCube())
		{
			m_Shaders.Lightcube.SetUniformMat4f("u_ModelMat", scene.GetPointLights()[scene.GetPointLightList()[i]].GetModelMat());
			m_Shaders.Lightcube.SetUniformVec3f("u_Color", scene.GetPointLights()[scene.GetPointLightList()[i]].GetColor());
			GLCall(glDrawElements(GL_TRIANGLES, scene.GetData().GetDataGL().GetLightCube(POINT_LIGHT).IB.GetCount(), GL_UNSIGNED_INT, nullptr));
		}
	}
	scene.GetData().GetDataGL().GetLightCube(POINT_LIGHT).VA.Unbind();
	scene.GetData().GetDataGL().GetLightCube(POINT_LIGHT).IB.Unbind();
	// Draw Spot light's cube
	scene.GetData().GetDataGL().GetLightCube(SPOT_LIGHT).VA.Bind();
	scene.GetData().GetDataGL().GetLightCube(SPOT_LIGHT).IB.Bind();
	for (unsigned int i = 0; i < scene.GetSpotLightList().size(); i++)
	{
		if (scene.GetSpotLights()[scene.GetSpotLightList()[i]].ShowCube())
		{
			m_Shaders.Lightcube.SetUniformMat4f("u_ModelMat", scene.GetSpotLights()[scene.GetSpotLightList()[i]].GetModelMat());
			m_Shaders.Lightcube.SetUniformVec3f("u_Color", scene.GetSpotLights()[scene.GetSpotLightList()[i]].GetColor());
			GLCall(glDrawElements(GL_TRIANGLES, scene.GetData().GetDataGL().GetLightCube(SPOT_LIGHT).IB.GetCount(), GL_UNSIGNED_INT, nullptr));
		}
	}
	scene.GetData().GetDataGL().GetLightCube(SPOT_LIGHT).VA.Unbind();
	scene.GetData().GetDataGL().GetLightCube(SPOT_LIGHT).IB.Unbind();
	// Draw Directional light's cube
	scene.GetData().GetDataGL().GetLightCube(DIRECTIONAL_LIGHT).VA.Bind();
	scene.GetData().GetDataGL().GetLightCube(DIRECTIONAL_LIGHT).IB.Bind();
	GLCall(glDisable(GL_DEPTH_TEST));
	for (unsigned int i = 0; i < scene.GetDirectionalLightList().size(); i++)
	{
		if (scene.GetDirectionalLights()[scene.GetDirectionalLightList()[i]].ShowCube())
		{
			glm::mat4 modelMat = scene.GetDirectionalLights()[scene.GetDirectionalLightList()[i]].GetModelMat()
								* glm::scale(glm::mat4(1.0f), glm::vec3(glm::length(scene.GetCamera().GetPosition()) / 20.0f));
			m_Shaders.Lightcube.SetUniformMat4f("u_ModelMat", modelMat);
			m_Shaders.Lightcube.SetUniformVec3f("u_Color", scene.GetDirectionalLights()[scene.GetDirectionalLightList()[i]].GetColor());
			GLCall(glDrawElements(GL_TRIANGLES, scene.GetData().GetDataGL().GetLightCube(DIRECTIONAL_LIGHT).IB.GetCount(), GL_UNSIGNED_INT, nullptr));
		}
	}
	scene.GetData().GetDataGL().GetLightCube(DIRECTIONAL_LIGHT).VA.Unbind();
	scene.GetData().GetDataGL().GetLightCube(DIRECTIONAL_LIGHT).IB.Unbind();
	GLCall(glEnable(GL_DEPTH_TEST));
	m_Shaders.Lightcube.Unbind();
}

bool GLRenderer::SaveScreenShot()
{
	// Make the BYTE array, factor of 3 because it's RBG.
	BYTE* pixels = new BYTE[3 * rbcore::SETTINGS.Width * rbcore::SETTINGS.Height];
	glReadPixels(0, 0, rbcore::SETTINGS.Width, rbcore::SETTINGS.Height, GL_BGR, GL_UNSIGNED_BYTE, pixels);
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
	FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, rbcore::SETTINGS.Width, rbcore::SETTINGS.Height, 3 * rbcore::SETTINGS.Width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
	if (!FreeImage_Save(FIF_PNG, image, filepath.c_str(), PNG_Z_BEST_SPEED))
	{
		rbcore::ShowWarningMsg("Unknow error! Couldn't save screenshot!");
		return false;
	}
	rbcore::ShowWarningMsg("Screenshot saved at: " + filepath);
	FreeImage_Unload(image);
	delete[] pixels;
	return true;
}

void GLRenderer::ChangeResolution()
{
	m_Frame.FB.ChangeResolution();
	if ((int)rbcore::SETTINGS.AA >= 1 && (int)rbcore::SETTINGS.AA <= 4)
	{
		m_Frame.FBMsaa.ChangeResolution();
	}
}

void GLRenderer::ChangeMSAA()
{
	m_Frame.FBMsaa.ChangeMSAA();
}

void GLRenderer::ChangePostProcess()
{
	m_Shaders.Screen.Bind();
	switch (rbcore::SETTINGS.PP)
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