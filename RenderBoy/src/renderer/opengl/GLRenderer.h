/*
* Class:
*	GLRenderer
* Description:
*	This class use OpenGL to draw everything in the scene on the screen
*/

#pragma once

#include "core/Core.h"
#include "scene/Scene.h"
#include "gl/opengl/GLFrameBuffer.h"
#include "shader/opengl/GLShader.h"

struct Frame
{
	GLFrameBuffer FB;
	GLFrameBuffer FBMsaa;
	GLVertexArray VA;
	GLVertexBuffer VB;
	GLIndexBuffer IB;
};

struct Shaders
{
	GLShader Normal;
	GLShader Lightcube;
	GLShader Screen;
};

class GLRenderer
{
private:
	Frame m_Frame;
	Shaders m_Shaders;

	void Clear();
	void DrawBlank(Scene& scene);
	void DrawWireFrame(Scene& scene);
	void DrawPointCloud(Scene& scene);
	void DrawDepth(Scene& scene);
	void DrawUVSet(Scene& scene);
	void DrawNormal(Scene& scene);
	void DrawLightCube(Scene& scene);
	void DrawSkybox(Scene& scene);
	void DrawPointLightShadow(Scene& scene);
	void DrawSpotLightShadow(Scene& scene);

public:
	GLRenderer();
	~GLRenderer();

	void Init(Scene& scene);
	void Draw(Scene& scene);
	
	bool SaveScreenShot();
	void ChangeResolution();
	void ChangeMSAA();
	void ChangePostProcess();
};