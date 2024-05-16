#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Init(Scene& scene)
{
	switch (rbcore::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_RendererGL.Init(scene);
		break;
	default:
		break;
	}
}

void Renderer::Draw(Scene& scene)
{
	switch (rbcore::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_RendererGL.Draw(scene);
		break;
	default:
		break;
	}
}

void Renderer::Clear()
{
	switch (rbcore::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		break;
	default:
		break;
	}
}

bool Renderer::SaveScreenShot()
{
	switch (rbcore::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		if (m_RendererGL.SaveScreenShot())
		{
			return true;
		}
		return false;
		break;
	default:
		break;
	}
	return false;
}

void Renderer::ChangeResolution()
{
	switch (rbcore::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_RendererGL.ChangeResolution();
		break;
	default:
		break;
	}
}

void Renderer::ChangeMSAA()
{
	switch (rbcore::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_RendererGL.ChangeMSAA();
		break;
	default:
		break;
	}
}

void Renderer::ChangePostProcess()
{
	switch (rbcore::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_RendererGL.ChangePostProcess();
		break;
	default:
		break;
	}
}

void Renderer::ChangeSkybox(Scene& scene)
{
	switch (rbcore::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_RendererGL.ChangeSkybox(scene);
		break;
	default:
		break;
	}
}
