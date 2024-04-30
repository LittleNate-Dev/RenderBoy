/*
* Class:
*	GLVertexArray
* Description:
*	This class contains all functions relate to vertex array object
*/

#pragma once

#include "core/Core.h"
#include "GLVertexBuffer.h"
#include "GLVertexBufferLayout.h"

class GLVertexBufferLayout;

class GLVertexArray
{
private:
    unsigned int m_RendererID;
    unsigned int m_Location;

public:
    GLVertexArray();
    ~GLVertexArray();

    // Tell opengl to generate a vertex array object
    void GenVertexArray();
    // Add a vertex buffer object to vertex array
    void AddBuffer(const GLVertexBuffer& vb, const GLVertexBufferLayout& layout);
    void AddBuffer(const GLVertexBuffer& vb, const GLVertexBufferLayout& layout, unsigned int divisor);
    void Bind() const;
    void Unbind() const;

    // Get GLVertexArray's members
    unsigned int GetID() const;
};