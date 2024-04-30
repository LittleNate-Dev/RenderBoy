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
	void GenIndexBuffer(const unsigned int* data, unsigned int count);
	void Bind() const;
	void Unbind() const;

	// Get IndexBuffer's members
	unsigned int GetID() const;
	unsigned int GetCount() const;
};