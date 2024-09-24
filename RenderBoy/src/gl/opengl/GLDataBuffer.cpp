#include "GLDataBuffer.h"

GLDataBuffer::GLDataBuffer()
{
	m_RendererID = 0;
}

GLDataBuffer::~GLDataBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void GLDataBuffer::GenBuffer(const void* data, unsigned int size, GLenum usage)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage));
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

void GLDataBuffer::GetData(void* data, int offset, unsigned int size)
{
	GLCall(glGetNamedBufferSubData(m_RendererID, offset, size, data));
}

void GLDataBuffer::SetData(const void* data, unsigned int size, GLenum usage)
{
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage));
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

void GLDataBuffer::Bind(unsigned int pos) const
{
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
	GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, pos, m_RendererID));
}

void GLDataBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}