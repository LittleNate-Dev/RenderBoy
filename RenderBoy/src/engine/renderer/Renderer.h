/*
* Class:
*	Renderer
* Description:
*	This class draws data on the screen
*/

#pragma once

#include "core/Core.h"
#include "opengl/RendererGL.h"

class Renderer
{
private:
	RendererGL m_RendererGL;

public:
	Renderer();
	~Renderer();

	void Init();
	void Draw(Scene& scene);
	void Clear();

	bool SaveScreenShot();
	void ChangeResolution();
	void ChangeMSAA();
	void ChangePostProcess();
};