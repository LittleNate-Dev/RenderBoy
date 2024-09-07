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
	G_BUFFER = 1,
	DEPTH_MAP = 2,
	DEPTH_CUBE = 3,
	SSAO = 4,
	OIT = 5
};

class GLFrameBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_RenderBufferID;
	std::vector<unsigned int> m_TexIDs;
	std::vector<GLuint64> m_Handles;
	FBType m_Type;
	glm::vec2 m_TexSize;

	void Destroy();

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
	inline unsigned int GetTexID(unsigned int slot = 0) const
	{
		return m_TexIDs[slot];
	};
	inline std::vector<unsigned int> GetTexIDs() const
	{
		return m_TexIDs;
	};
	inline FBType GetType() const
	{
		return m_Type;
	};
	inline GLuint64 GetHandle(unsigned int slot = 0) const
	{
		return m_Handles[slot];
	};
	inline std::vector<GLuint64> GetHandles() const
	{
		return m_Handles;
	};
	inline unsigned int GetTexWidth() const
	{
		return (unsigned int)m_TexSize.x;
	};
	inline unsigned int GetTexHeight() const
	{
		return (unsigned int)m_TexSize.y;
	};
	inline glm::vec2 GetTexSize() const
	{
		return m_TexSize;
	};
};