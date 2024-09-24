/*
* Class:
*	IndexBuffer
* Description:
*	This class contains all functions relate to index buffer
*/

#pragma once

#include "core/Core.h"

class GLIndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;

public:
	GLIndexBuffer();
	~GLIndexBuffer();

	// Tell opengl to generate an index buffer object
	void GenBuffer(const unsigned int* data, unsigned int count);
	void Bind() const;
	void Unbind() const;

	// Get IndexBuffer's members
	inline unsigned int GetID() const
	{
		return m_RendererID;
	};
	inline unsigned int GetCount() const
	{
		return m_Count;
	};
};