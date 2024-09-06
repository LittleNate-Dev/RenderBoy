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
	GLFrameBuffer GBuffer;
	GLFrameBuffer Bloom[7];
	GLFrameBuffer SSAO[2];
	GLVertexArray VA;
	GLVertexBuffer VB;
	GLIndexBuffer IB;
};

struct Shaders
{
	GLShader GBuffer;
	GLShader Normal;
	GLShader Lightcube;
	GLShader Screen;
	GLShader Bloom[3];
	GLShader SSAO[2];
};

class GLRenderer
{
private:
	Frame m_Frame;
	Shaders m_Shaders;

	void UpdateModelMat(Scene& scene);
	void Clear();
	void DrawDefault(Scene& scene);
	void DrawGBuffer(Scene& scene);
	void DrawBlank(Scene& scene);
	void DrawWireFrame(Scene& scene);
	void DrawPointCloud(Scene& scene);
	void DrawDepth(Scene& scene);
	void DrawNormalDM(Scene& scene); //Draw mode
	void DrawUVSet(Scene& scene);
	void DrawNormal(Scene& scene);
	void DrawLightCube(Scene& scene);
	void DrawSkybox(Scene& scene);
	void DrawPointLightShadow(Scene& scene, bool update);
	void DrawSpotLightShadow(Scene& scene, bool update);
	void DrawDirLightShadow(Scene& scene);
	void DrawBloom(Scene& scene);
	void DrawSSAO(Scene& scene);

public:
	GLRenderer();
	~GLRenderer();

	void Init(Scene& scene);
	void Draw(Scene& scene);
	
	bool SaveScreenShot(Scene& scene);
	void ChangeResolution();
	void ChangeMSAA();
	void ChangePostProcess();
};