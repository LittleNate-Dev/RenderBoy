/*
* Class:
*	GLDataBuffer
* Description:
*	This class contains all functions relate to data buffer, which is used for compute shader
*/

#pragma once

#include "core/Core.h"

class GLDataBuffer
{
private:
	unsigned int m_RendererID;

public:
	GLDataBuffer();
	~GLDataBuffer();

	// Tell opengl to generate an index buffer object
	void GenBuffer(const void* data, unsigned int size, GLenum usage);
	void GetData(void* data, int offset, unsigned int size);
	void SetData(const void* data, unsigned int size, GLenum usage);
	void Bind(unsigned int pos = 0) const;
	void Unbind() const;

	// Get IndexBuffer's members
	inline unsigned int GetID() const
	{
		return m_RendererID;
	};
};