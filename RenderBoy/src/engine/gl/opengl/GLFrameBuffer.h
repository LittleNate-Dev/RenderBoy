/*
* Class:
*	GLFrameBuffer
* Description:
*	Just Frame Buffer, I'm tired of writing comments :(
*/

#pragma once

#include "core/Core.h"

enum FBType
{
	FRAME = 0,
	MSAA = 1,
	DEPTH_MAP = 2,
	DEPTH_CUBE = 3
};

class GLFrameBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_RenderBufferID;
	unsigned int m_TexID;
	FBType m_Type;

public:
	GLFrameBuffer();
	~GLFrameBuffer();

	// width and height is window's size, not msaa size!
	void Init(FBType type);
	void Init(FBType type, unsigned int width, unsigned int height);
	bool IsInitialized();
	// Bind and Unbind
	void Bind() const;
	void Unbind() const;
	void BindTex() const;
	void UnbindTex() const;
	// Window resize
	void ChangeResolution();
	void ChangeMSAA();
	// Get GLFrameBuffer's members
	unsigned int GetID();
	unsigned int GetRenderBufferID();
	unsigned int GetTexID();
	FBType GetType();
};