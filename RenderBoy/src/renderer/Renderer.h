/*
* Class:
*	Renderer
* Description:
*	This class draws data on the screen
*/

#pragma once

#include "core/Core.h"
#include "opengl/GLRenderer.h"

class Renderer
{
private:
	GLRenderer m_RendererGL;

public:
	Renderer();
	~Renderer();

	bool Init(Scene& scene);
	void Draw(Scene& scene);
	void Clear();

	bool SaveScreenShot(Scene& scene);
	void ChangeResolution();
	bool ChangePostProcess();
	bool ChangeAA();
};