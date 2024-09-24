/*
* Class:
*   GLVertexBuffer
* Description:
*   This class cotains all functions relate to vertex buffer
*/

#pragma once

#include "core/Core.h"

class GLVertexBuffer
{
private:
    unsigned int m_RendererID;
    
public:
    GLVertexBuffer();
    ~GLVertexBuffer();

    // Tell opengl to generate a vertex buffer object
    void GenBuffer(const void* data, unsigned int size);
    void UpdateBuffer(const void* data, unsigned int size);
    void Bind() const;
    void Unbind() const;

    // Get GLVertexBuffer's members
    inline unsigned int GetID() const
    {
        return m_RendererID;
    };
};
