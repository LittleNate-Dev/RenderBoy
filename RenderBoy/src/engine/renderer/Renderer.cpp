#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Init()
{
	switch (rbcore::SETTINGS.core)
	{
	case OPENGL:
		m_RendererGL.Init();
		break;
	default:
		break;
	}
}

void Renderer::Draw(Scene& scene)
{
	switch (rbcore::SETTINGS.core)
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
	switch (rbcore::SETTINGS.core)
	{
	case OPENGL:
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		break;
	default:
		break;
	}
}

void Renderer::ChangeResolution()
{
	switch (rbcore::SETTINGS.core)
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
	switch (rbcore::SETTINGS.core)
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
	switch (rbcore::SETTINGS.core)
	{
	case OPENGL:
		m_RendererGL.ChangePostProcess();
		break;
	default:
		break;
	}
}