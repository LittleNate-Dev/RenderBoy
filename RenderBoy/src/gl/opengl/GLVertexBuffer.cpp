#include "GLVertexBuffer.h"

GLVertexBuffer::GLVertexBuffer()
{
    m_RendererID = 0;
}

GLVertexBuffer::~GLVertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

//public
void GLVertexBuffer::GenVertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    Bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    Unbind();
}

void GLVertexBuffer::UpdateVertexBuffer(const void* data, unsigned int size)
{
    Bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    Unbind();
}

void GLVertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void GLVertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}