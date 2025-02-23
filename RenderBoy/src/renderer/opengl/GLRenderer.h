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
	GLVertexArray VA;
	GLVertexBuffer VB;
	GLIndexBuffer IB;
	GLFrameBuffer FB;
	GLFrameBuffer Screen;
	GLFrameBuffer OIT;
	GLFrameBuffer GBuffer;
	GLFrameBuffer Bloom[7];
	GLFrameBuffer SSAO[2];
	GLFrameBuffer DOF[4];
	GLFrameBuffer FXAA;
};

struct Shaders
{
	GLShader OIT;
	GLShader GBufferArea;
	GLShader Normal;
	GLShader Lightcube;
	GLShader LightcubeArea;
	GLShader Screen;
	GLShader PP;
	GLShader Bloom[3]; // 0: DOWNSAMPLE, 1: UPSAMPLE, 2: BLEND
	GLShader SSAO[2]; // 0: GEN, 1: BLUR
	GLShader DOF[4]; // 0: COC, 1: BOKEH, 2: DOWNSAMPLE, 3: BLEND
	GLShader GaussianBlur;
	GLShader FXAA;
	GLShader Exposure[2]; // 0£º HISTOGRAM, 1: AVERAGE
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
	void DrawAreaLightCube(Scene& scene, AL_Type type);
	void DrawSkybox(Scene& scene);
	void DrawPointLightShadow(Scene& scene, bool update);
	void DrawSpotLightShadow(Scene& scene, bool update);
	void DrawDirLightShadow(Scene& scene);
	void DrawDOF(Scene& scene); // Depth of Field
	void DrawBloom(Scene& scene);
	void DrawSSAO(Scene& scene);
	void DrawAA(Scene& scene); // Use this function to apply all post-processing aa
	void DrawFXAA(Scene& scene);
	void DrawSMAA(Scene& scene);
	void DrawAutoExposure(Scene& scene);
	void DrawToneMapping(Scene& scene);

public:
	GLRenderer();
	~GLRenderer();

	bool Init(Scene& scene);
	void Draw(Scene& scene);
	
	bool SaveScreenShot(Scene& scene);
	void ChangeResolution();
	bool ChangePostProcess();
};