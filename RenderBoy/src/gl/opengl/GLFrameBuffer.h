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
	GLuint64 m_Handle;
	FBType m_Type;
	unsigned int m_TexWidth;
	unsigned int m_TexHeight;

public:
	GLFrameBuffer();
	~GLFrameBuffer();

	// width and height is window's size, not msaa size!
	void Init(FBType type);
	// width and height in this function mean shadow resolution
	void Init(FBType type, unsigned int width, unsigned int height);
	bool IsInitialized();
	// Bind and Unbind
	void Bind() const;
	void Unbind() const;
	void BindTex(unsigned int slot = 0) const;
	void UnbindTex() const;
	// Window resize
	void ChangeResolution();
	void ChangeMSAA();
	void ChangeShadowRes(unsigned int width, unsigned int height);
	// Get GLFrameBuffer's members
	inline unsigned int GetID() const
	{
		return m_RendererID;
	};
	inline unsigned int GetRenderBufferID() const
	{
		return m_RenderBufferID;
	};
	inline unsigned int GetTexID() const
	{
		return m_TexID;
	};
	inline FBType GetType() const
	{
		return m_Type;
	};
	inline GLuint64 GetHandle() const
	{
		return m_Handle;
	};
	inline unsigned int GetTexWidth() const
	{
		return m_TexWidth;
	};
	inline unsigned int GetTexHeight() const
	{
		return m_TexHeight;
	};
};