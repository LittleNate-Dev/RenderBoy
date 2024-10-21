#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::Init(Scene& scene)
{
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		if (!m_RendererGL.Init(scene))
		{
			return false;
		}
		break;
	default:
		break;
	}
	return true;
}

void Renderer::Draw(Scene& scene)
{
	switch (core::SETTINGS.GraphicsCore)
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
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		break;
	default:
		break;
	}
}

bool Renderer::SaveScreenShot(Scene& scene)
{
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		if (m_RendererGL.SaveScreenShot(scene))
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
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_RendererGL.ChangeResolution();
		break;
	default:
		break;
	}
}

bool Renderer::ChangePostProcess()
{
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		if (!m_RendererGL.ChangePostProcess())
		{
			return false;
		}
		break;
	default:
		break;
	}
	return true;
}