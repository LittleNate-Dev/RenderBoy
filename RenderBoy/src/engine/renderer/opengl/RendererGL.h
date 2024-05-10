/*
* Class:
*	RendererGL
* Description:
*	This class use OpenGL to draw everything in the scene on the screen
*/

#pragma once

#include "core/Core.h"
#include "scene/Scene.h"
#include "gl/opengl/GLFrameBuffer.h"
#include "shader/opengl/ShaderGL.h"

struct Frame
{
	GLFrameBuffer fb;
	GLFrameBuffer fbMsaa;
	GLVertexArray va;
	GLVertexBuffer vb;
	GLIndexBuffer ib;
};

struct Shaders
{
	ShaderGL wireframe;
	ShaderGL pointcloud;
	ShaderGL depth;
	ShaderGL normal;
	ShaderGL screen;
};

class RendererGL
{
private:
	Frame m_Frame;
	Shaders m_Shaders;

	void Clear();
	void DrawWireFrame(Scene& scene);
	void DrawPointCloud(Scene& scene);
	void DrawDepth(Scene& scene);
	void DrawNormal(Scene& scene);

public:
	RendererGL();
	~RendererGL();

	void Init();
	void Draw(Scene& scene);
	
	bool SaveScreenShot();
	void ChangeResolution();
	void ChangeMSAA();
	void ChangePostProcess();
};