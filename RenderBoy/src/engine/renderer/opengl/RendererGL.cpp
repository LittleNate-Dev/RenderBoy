#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "RendererGL.h"
#include <stb/stb_image_write.h>

RendererGL::RendererGL()
{
}

RendererGL::~RendererGL()
{
}

void RendererGL::Init()
{
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glLineWidth(0.4f));
	GLCall(glPointSize(1.5f));
	// Initialize shaders
	m_Shaders.wireframe.Init((std::string)SHADER_OPENGL + "WIREFRAME.glsl");
	m_Shaders.pointcloud.Init((std::string)SHADER_OPENGL + "POINTCLOUD.glsl");
	m_Shaders.depth.Init((std::string)SHADER_OPENGL + "DEPTH.glsl");
	m_Shaders.normal.Init((std::string)SHADER_OPENGL + "NORMAL.glsl");
	m_Shaders.lightcube.Init((std::string)SHADER_OPENGL + "LIGHTCUBE.glsl");
	ChangePostProcess();
	// Initialize frame buffers
	m_Frame.fbMsaa.Init(FBType::MSAA);
	if (!m_Frame.fbMsaa.IsInitialized())
	{
		rbcore::ShowWarningMsg("Failed to initialize OpenGL renderer!");
	}
	m_Frame.fb.Init(FBType::FRAME);
	if (m_Frame.fb.IsInitialized())
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
		m_Frame.va.GenVertexArray();
		m_Frame.vb.GenVertexBuffer(position, sizeof(position));
		m_Frame.ib.GenIndexBuffer(indices, 6);
		GLVertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_Frame.va.AddBuffer(m_Frame.vb, layout);
	}
	else
	{
		rbcore::ShowWarningMsg("Failed to initialize OpenGL renderer!");
	}	
}

void RendererGL::Clear()
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void RendererGL::Draw(Scene& scene)
{
	// MSAA
	if ((int)rbcore::SETTINGS.aa >= 1 && (int)rbcore::SETTINGS.aa <= 4)
	{
		m_Frame.fbMsaa.Bind();
	}
	else
	{
		m_Frame.fb.Bind();
	}
	// Draw your content inside this scope
	Clear();
	glm::vec2 renderRes = rbcore::GetRenderResolution();
	GLCall(glViewport(0, 0, (GLsizei)renderRes.x, (GLsizei)renderRes.y));
	switch (rbcore::SETTINGS.drawMode)
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
	}
	// Draw Light Cube
	DrawLightCube(scene);
	// Draw normals
	if (rbcore::SETTINGS.showNormal)
	{
		DrawNormal(scene);
	}
	// Draw your content inside this scope
	if ((int)rbcore::SETTINGS.aa >= 1 && (int)rbcore::SETTINGS.aa <= 4)
	{
		GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Frame.fbMsaa.GetID()));
		GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Frame.fb.GetID()));
		int width = (int)(rbcore::SETTINGS.width * rbcore::SETTINGS.resolution);
		int height = (int)(rbcore::SETTINGS.height * rbcore::SETTINGS.resolution);
		GLCall(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
	}
	m_Frame.fb.Unbind();
	Clear();
	GLCall(glViewport(0, 0, rbcore::SETTINGS.width, rbcore::SETTINGS.height));
	// Draw frame buffer's content on the screen;
	GLCall(glDisable(GL_DEPTH_TEST));
	m_Shaders.screen.Bind();
	m_Frame.fb.BindTex();
	m_Shaders.screen.SetUniform1i("u_ScreenTex", 0);
	m_Shaders.screen.SetUniform1f("u_Gamma", rbcore::SETTINGS.gamma);
	m_Frame.va.Bind();
	m_Frame.ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_Frame.ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	m_Frame.va.Unbind();
	m_Frame.ib.Unbind();
	m_Shaders.screen.Unbind();
	m_Frame.fb.UnbindTex();
	GLCall(glEnable(GL_DEPTH_TEST));
}

void RendererGL::DrawWireFrame(Scene& scene)
{
	std::string model;
	m_Shaders.wireframe.Bind();
	m_Shaders.wireframe.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.wireframe.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	glm::vec2 renderRes = rbcore::GetRenderResolution();
	m_Shaders.wireframe.SetUniformMat4f("u_ViewPortMat", rbcore::GetViewPortMatrix((unsigned int)renderRes.x, (unsigned int)renderRes.y));
	m_Shaders.wireframe.SetUniformVec2f("u_RenderRes", renderRes);
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].instanceVb.UpdateVertexBuffer(
			&scene.GetModels()[model].GetModelMats()[0], 
			(unsigned int)scene.GetModels()[model].GetModelMats().size() * sizeof(glm::mat4)
		);
		scene.GetData().GetDataGL().GetModelData()[model].va.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].ib.Bind();
		// Draw Model
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].ib.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].va.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].ib.Unbind();
	}
	m_Shaders.wireframe.Unbind();
}

void RendererGL::DrawPointCloud(Scene& scene)
{
	std::string model;
	m_Shaders.pointcloud.Bind();
	m_Shaders.pointcloud.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.pointcloud.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].instanceVb.UpdateVertexBuffer(
			&scene.GetModels()[model].GetModelMats()[0],
			(unsigned int)scene.GetModels()[model].GetModelMats().size() * sizeof(glm::mat4)
		);
		scene.GetData().GetDataGL().GetModelData()[model].va.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].ib.Bind();
		GLCall(glDrawElementsInstanced(GL_POINTS, scene.GetData().GetDataGL().GetModelData()[model].ib.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].va.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].ib.Unbind();
	}
	m_Shaders.pointcloud.Unbind();
}

void RendererGL::DrawDepth(Scene& scene)
{
	std::string model;
	m_Shaders.depth.Bind();
	m_Shaders.depth.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.depth.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	m_Shaders.depth.SetUniform1f("u_NearPlane", scene.GetCamera().GetNearPlane());
	m_Shaders.depth.SetUniform1f("u_FarPlane", scene.GetCamera().GetFarPlane());
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].instanceVb.UpdateVertexBuffer(
			&scene.GetModels()[model].GetModelMats()[0],
			(unsigned int)scene.GetModels()[model].GetModelMats().size() * sizeof(glm::mat4)
		);
		scene.GetData().GetDataGL().GetModelData()[model].va.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].ib.Bind();
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].ib.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].va.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].ib.Unbind();
	}
	m_Shaders.pointcloud.Unbind();
}

void RendererGL::DrawNormal(Scene& scene)
{
	std::string model;
	m_Shaders.normal.Bind();
	m_Shaders.normal.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.normal.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	m_Shaders.normal.SetUniform1f("u_Magnitude", rbcore::NORMAL_MAGNITUDE);
	m_Shaders.normal.SetUniformVec3f("u_Color", rbcore::NORMAL_COLOR);
	for (unsigned int i = 0; i < scene.GetModelList().size(); i++)
	{
		model = scene.GetModelList()[i];
		scene.GetData().GetDataGL().GetModelData()[model].instanceVb.UpdateVertexBuffer(
			&scene.GetModels()[model].GetModelMats()[0],
			(unsigned int)scene.GetModels()[model].GetModelMats().size() * sizeof(glm::mat4)
		);
		scene.GetData().GetDataGL().GetModelData()[model].va.Bind();
		scene.GetData().GetDataGL().GetModelData()[model].ib.Bind();
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, scene.GetData().GetDataGL().GetModelData()[model].ib.GetCount(), GL_UNSIGNED_INT, nullptr, scene.GetModels()[model].GetInstance()));
		scene.GetData().GetDataGL().GetModelData()[model].va.Unbind();
		scene.GetData().GetDataGL().GetModelData()[model].ib.Unbind();
	}
	m_Shaders.normal.Unbind();
}

void RendererGL::DrawLightCube(Scene& scene)
{
	m_Shaders.lightcube.Bind();
	glm::vec2 renderRes = rbcore::GetRenderResolution();
	m_Shaders.lightcube.SetUniformMat4f("u_ProjMat", scene.GetCamera().GetProjMat());
	m_Shaders.lightcube.SetUniformMat4f("u_ViewMat", scene.GetCamera().GetViewMat());
	m_Shaders.lightcube.SetUniformMat4f("u_ViewPortMat", rbcore::GetViewPortMatrix((unsigned int)renderRes.x, (unsigned int)renderRes.y));
	m_Shaders.lightcube.SetUniformVec2f("u_RenderRes", renderRes);
	// Draw Point light's cube
	scene.GetData().GetDataGL().GetLightCube(POINT_LIGHT).va.Bind();
	scene.GetData().GetDataGL().GetLightCube(POINT_LIGHT).ib.Bind();
	for (unsigned int i = 0; i < scene.GetPointLightList().size(); i++)
	{
		if (scene.GetPointLights()[scene.GetPointLightList()[i]].ShowCube())
		{
			m_Shaders.lightcube.SetUniformMat4f("u_ModelMat", scene.GetPointLights()[scene.GetPointLightList()[i]].GetModelMat());
			m_Shaders.lightcube.SetUniformVec3f("u_Color", scene.GetPointLights()[scene.GetPointLightList()[i]].GetColor());
			GLCall(glDrawElements(GL_TRIANGLES, scene.GetData().GetDataGL().GetLightCube(POINT_LIGHT).ib.GetCount(), GL_UNSIGNED_INT, nullptr));
		}
	}
	scene.GetData().GetDataGL().GetLightCube(POINT_LIGHT).va.Unbind();
	scene.GetData().GetDataGL().GetLightCube(POINT_LIGHT).ib.Unbind();
	// Draw Spot light's cube
	scene.GetData().GetDataGL().GetLightCube(SPOT_LIGHT).va.Bind();
	scene.GetData().GetDataGL().GetLightCube(SPOT_LIGHT).ib.Bind();
	for (unsigned int i = 0; i < scene.GetSpotLightList().size(); i++)
	{
		if (scene.GetSpotLights()[scene.GetSpotLightList()[i]].ShowCube())
		{
			m_Shaders.lightcube.SetUniformMat4f("u_ModelMat", scene.GetSpotLights()[scene.GetSpotLightList()[i]].GetModelMat());
			m_Shaders.lightcube.SetUniformVec3f("u_Color", scene.GetSpotLights()[scene.GetSpotLightList()[i]].GetColor());
			GLCall(glDrawElements(GL_TRIANGLES, scene.GetData().GetDataGL().GetLightCube(SPOT_LIGHT).ib.GetCount(), GL_UNSIGNED_INT, nullptr));
		}
	}
	scene.GetData().GetDataGL().GetLightCube(SPOT_LIGHT).va.Unbind();
	scene.GetData().GetDataGL().GetLightCube(SPOT_LIGHT).ib.Unbind();
	// Draw Directional light's cube
	scene.GetData().GetDataGL().GetLightCube(DIRECTIONAL_LIGHT).va.Bind();
	scene.GetData().GetDataGL().GetLightCube(DIRECTIONAL_LIGHT).ib.Bind();
	GLCall(glDisable(GL_DEPTH_TEST));
	for (unsigned int i = 0; i < scene.GetDirectionalLightList().size(); i++)
	{
		if (scene.GetDirectionalLights()[scene.GetDirectionalLightList()[i]].ShowCube())
		{
			glm::mat4 modelMat = scene.GetDirectionalLights()[scene.GetDirectionalLightList()[i]].GetModelMat()
								* glm::scale(glm::mat4(1.0f), glm::vec3(glm::length(scene.GetCamera().GetPosition()) / 20.0f));
			m_Shaders.lightcube.SetUniformMat4f("u_ModelMat", modelMat);
			m_Shaders.lightcube.SetUniformVec3f("u_Color", scene.GetDirectionalLights()[scene.GetDirectionalLightList()[i]].GetColor());
			GLCall(glDrawElements(GL_TRIANGLES, scene.GetData().GetDataGL().GetLightCube(DIRECTIONAL_LIGHT).ib.GetCount(), GL_UNSIGNED_INT, nullptr));
		}
	}
	scene.GetData().GetDataGL().GetLightCube(DIRECTIONAL_LIGHT).va.Unbind();
	scene.GetData().GetDataGL().GetLightCube(DIRECTIONAL_LIGHT).ib.Unbind();
	GLCall(glEnable(GL_DEPTH_TEST));
	m_Shaders.lightcube.Unbind();
}

bool RendererGL::SaveScreenShot()
{
	glm::vec2 renderRes = rbcore::GetRenderResolution();
	std::unique_ptr<unsigned char[]> data = std::make_unique<unsigned char[]>(renderRes.x * renderRes.y * 3 * sizeof(unsigned int));
	m_Frame.fb.BindTex();
	GLCall(glPixelStorei(GL_PACK_ALIGNMENT, 1));
	GLCall(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data.get()));
	stbi_flip_vertically_on_write(true);
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
							+ ".jpg";
	int ret = stbi_write_jpg(filepath.c_str(), renderRes.x, renderRes.y, 3, data.get(), 100);
	if (ret == 0)
	{
		rbcore::ShowWarningMsg("Unknow Error! Can't save screenshot!");
		return false;
	}
	rbcore::ShowWarningMsg("Screenshot saved at: " + filepath);
	return true;
}

void RendererGL::ChangeResolution()
{
	m_Frame.fb.ChangeResolution();
	if ((int)rbcore::SETTINGS.aa >= 1 && (int)rbcore::SETTINGS.aa <= 4)
	{
		m_Frame.fbMsaa.ChangeResolution();
	}
}

void RendererGL::ChangeMSAA()
{
	m_Frame.fbMsaa.ChangeMSAA();
}

void RendererGL::ChangePostProcess()
{
	m_Shaders.screen.Bind();
	switch (rbcore::SETTINGS.pp)
	{
	case NO_PP:
		m_Shaders.screen.Init((std::string)SHADER_OPENGL + "post_process/DEFAULT.glsl");
		break;
	case INVERSE:
		m_Shaders.screen.Init((std::string)SHADER_OPENGL + "post_process/INVERSE.glsl");
		break;
	case BLUR:
		m_Shaders.screen.Init((std::string)SHADER_OPENGL + "post_process/BLUR.glsl");
		break;
	case GRAY_SCALE:
		m_Shaders.screen.Init((std::string)SHADER_OPENGL + "post_process/GRAYSCALE.glsl");
		break;
	case EDGE:
		m_Shaders.screen.Init((std::string)SHADER_OPENGL + "post_process/EDGE.glsl");
		break;
	default:
		m_Shaders.screen.Init((std::string)SHADER_OPENGL + "post_process/DEFAULT.glsl");
		break;
	}
	m_Shaders.screen.Unbind();
}