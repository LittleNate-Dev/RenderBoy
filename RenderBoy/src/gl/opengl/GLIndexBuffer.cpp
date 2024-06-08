#include "GLIndexBuffer.h"

GLIndexBuffer::GLIndexBuffer()
{
    m_RendererID = 0;
    m_Count = 0;
}

GLIndexBuffer::~GLIndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

//public
void GLIndexBuffer::GenIndexBuffer(const unsigned int* data, unsigned int count)
{
    m_Count = count;
    GLCall(glGenBuffers(1, &m_RendererID));
    Bind();
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
    Unbind();
}

void GLIndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void GLIndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}