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
#include "data/opengl/texture/GLTexture.h"

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
	GLShader Wireframe;
	GLShader Pointcloud;
	GLShader Depth;
	GLShader Normal;
	GLShader UVset;
	GLShader Lightcube;
	GLShader Screen;
	GLShader Skybox;
	GLShader PointShadow;
	GLShader SpotShadow;
	GLShader DirShadow;
};

class GLRenderer
{
private:
	Frame m_Frame;
	Shaders m_Shaders;
	GLTexture m_CheckerMap;

	void Clear();
	void DrawWireFrame(Scene& scene);
	void DrawPointCloud(Scene& scene);
	void DrawDepth(Scene& scene);
	void DrawUVSet(Scene& scene);
	void DrawNormal(Scene& scene);
	void DrawLightCube(Scene& scene);
	void DrawSkybox(Scene& scene);
	void DrawPointLightShadow(Scene& scene);

public:
	GLRenderer();
	~GLRenderer();

	void Init(Scene& scene);
	void Draw(Scene& scene);
	
	bool SaveScreenShot();
	void ChangeResolution();
	void ChangeMSAA();
	void ChangePostProcess();
	void ChangeSkybox(Scene& scene);
};