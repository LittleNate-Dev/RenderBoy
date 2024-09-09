#include "GLFrameBuffer.h"

GLFrameBuffer::GLFrameBuffer()
{
	m_RendererID = 0;
	m_RenderBufferID = 0;
	m_TexSize = glm::vec2(0);
}

GLFrameBuffer::~GLFrameBuffer()
{
	glDeleteFramebuffers(1, &m_RendererID);
	glDeleteRenderbuffers(1, &m_RenderBufferID);
	for (unsigned int i = 0; i < m_Handles.size(); i++)
	{
		glMakeTextureHandleNonResidentARB(m_Handles[i]);
		glDeleteTextures(1, &m_TexIDs[i]);
	}
	m_RendererID = 0;
	m_RenderBufferID = 0;
	std::vector<unsigned int>().swap(m_TexIDs);
	std::vector<GLuint64>().swap(m_Handles);
	m_TexSize = glm::vec2(0);
}

void GLFrameBuffer::Destroy()
{
	GLCall(glDeleteFramebuffers(1, &m_RendererID));
	GLCall(glDeleteRenderbuffers(1, &m_RenderBufferID));
	for (unsigned int i = 0; i < m_Handles.size(); i++)
	{
		GLCall(glMakeTextureHandleNonResidentARB(m_Handles[i]));
		GLCall(glDeleteTextures(1, &m_TexIDs[i]));
	}
	m_RendererID = 0;
	m_RenderBufferID = 0;
	std::vector<unsigned int>().swap(m_TexIDs);
	std::vector<GLuint64>().swap(m_Handles);
	m_TexSize = glm::vec2(0);
}

void GLFrameBuffer::Init(FBType type)
{
	int width = (int)(core::SETTINGS.Width * core::SETTINGS.Resolution);
	int height = (int)(core::SETTINGS.Height * core::SETTINGS.Resolution);

	Destroy();
	m_TexSize = glm::vec2(width, height);
	m_Type = type;
	GLCall(glGenFramebuffers(1, &m_RendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	if (m_Type == FRAME)
	{
		// Render buffer
		GLCall(glGenRenderbuffers(1, &m_RenderBufferID));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		// Color buffer
		m_TexIDs.push_back(0);
		GLCall(glGenTextures(1, &m_TexIDs[0]));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexIDs[0], 0));
	}
	else if (m_Type == OIT)
	{
		// Render buffer
		GLCall(glGenRenderbuffers(1, &m_RenderBufferID));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID));
		// accum buffer
		m_TexIDs.push_back(0);
		GLCall(glGenTextures(1, &m_TexIDs[0]));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_HALF_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		m_Handles.push_back(0);
		m_Handles[0] = glGetTextureHandleARB(m_TexIDs[0]);
		GLCall(glMakeTextureHandleResidentARB(m_Handles[0]));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexIDs[0], 0));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		// reveal color buffer
		m_TexIDs.push_back(0);
		GLCall(glGenTextures(1, &m_TexIDs[1]));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[1]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		m_Handles.push_back(0);
		m_Handles[1] = glGetTextureHandleARB(m_TexIDs[1]);
		GLCall(glMakeTextureHandleResidentARB(m_Handles[1]));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_TexIDs[1], 0));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		GLCall(glDrawBuffers(2, attachments));
	}
	else if (m_Type == G_BUFFER)
	{
		// Render buffer
		GLCall(glGenRenderbuffers(1, &m_RenderBufferID));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID));
		// Position color buffer
		m_TexIDs.push_back(0);
		GLCall(glGenTextures(1, &m_TexIDs[0]));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		m_Handles.push_back(0);
		m_Handles[0] = glGetTextureHandleARB(m_TexIDs[0]);
		GLCall(glMakeTextureHandleResidentARB(m_Handles[0]));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexIDs[0], 0));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		// Normal color buffer
		m_TexIDs.push_back(0);
		GLCall(glGenTextures(1, &m_TexIDs[1]));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[1]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		m_Handles.push_back(0);
		m_Handles[1] = glGetTextureHandleARB(m_TexIDs[1]);
		GLCall(glMakeTextureHandleResidentARB(m_Handles[1]));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_TexIDs[1], 0));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		// Albedo color buffer
		m_TexIDs.push_back(0);
		GLCall(glGenTextures(1, &m_TexIDs[2]));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[2]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		m_Handles.push_back(0);
		m_Handles[2] = glGetTextureHandleARB(m_TexIDs[2]);
		GLCall(glMakeTextureHandleResidentARB(m_Handles[2]));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_TexIDs[2], 0));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		GLCall(glDrawBuffers(3, attachments));
	}
	else if (m_Type == SSAO)
	{
		m_TexIDs.push_back(0);
		GLCall(glGenTextures(1, &m_TexIDs[0]));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexIDs[0], 0));
		m_Handles.push_back(0);
		m_Handles[0] = glGetTextureHandleARB(m_TexIDs[0]);
		GLCall(glMakeTextureHandleResidentARB(m_Handles[0]));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void GLFrameBuffer::Init(FBType type, unsigned int width, unsigned int height)
{
	Destroy();
	m_Type = type;
	m_TexSize = glm::vec2(width, height);
	GLCall(glGenFramebuffers(1, &m_RendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	
	if (m_Type == FRAME)
	{
		// Render buffer
		GLCall(glGenRenderbuffers(1, &m_RenderBufferID));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		// Color buffer
		m_TexIDs.push_back(0);
		GLCall(glGenTextures(1, &m_TexIDs[0]));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexIDs[0], 0));
	}
	else if (m_Type == G_BUFFER)
	{
		// Render buffer
		GLCall(glGenRenderbuffers(1, &m_RenderBufferID));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID));
		// Position color buffer
		m_TexIDs.push_back(0);
		GLCall(glGenTextures(1, &m_TexIDs[0]));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		m_Handles.push_back(0);
		m_Handles[0] = glGetTextureHandleARB(m_TexIDs[0]);
		GLCall(glMakeTextureHandleResidentARB(m_Handles[0]));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexIDs[0], 0));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		// Normal color buffer
		m_TexIDs.push_back(0);
		GLCall(glGenTextures(1, &m_TexIDs[1]));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[1]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		m_Handles.push_back(0);
		m_Handles[1] = glGetTextureHandleARB(m_TexIDs[1]);
		GLCall(glMakeTextureHandleResidentARB(m_Handles[1]));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_TexIDs[1], 0));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		// Albedo color buffer
		m_TexIDs.push_back(0);
		GLCall(glGenTextures(1, &m_TexIDs[2]));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[2]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		m_Handles.push_back(0);
		m_Handles[2] = glGetTextureHandleARB(m_TexIDs[2]);
		GLCall(glMakeTextureHandleResidentARB(m_Handles[2]));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_TexIDs[2], 0));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		GLCall(glDrawBuffers(3, attachments));
	}
	else if (m_Type == OIT)
	{
		// Render buffer
		GLCall(glGenRenderbuffers(1, &m_RenderBufferID));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID));
		// Accum buffer
		m_TexIDs.push_back(0);
		GLCall(glGenTextures(1, &m_TexIDs[0]));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_HALF_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		m_Handles.push_back(0);
		m_Handles[0] = glGetTextureHandleARB(m_TexIDs[0]);
		GLCall(glMakeTextureHandleResidentARB(m_Handles[0]));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexIDs[0], 0));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		// Reveal color buffer
		m_TexIDs.push_back(0);
		GLCall(glGenTextures(1, &m_TexIDs[1]));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[1]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		m_Handles.push_back(0);
		m_Handles[1] = glGetTextureHandleARB(m_TexIDs[1]);
		GLCall(glMakeTextureHandleResidentARB(m_Handles[1]));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_TexIDs[1], 0));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		GLCall(glDrawBuffers(2, attachments));
	}
	else if (m_Type == DEPTH_MAP)
	{
		m_TexIDs.push_back(0);
		GLCall(glGenTextures(1, &m_TexIDs[0]));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_TexIDs[0], 0));
		// Generate texture handle
		m_Handles.push_back(0);
		m_Handles[0] = glGetTextureHandleARB(m_TexIDs[0]);
		GLCall(glMakeTextureHandleResidentARB(m_Handles[0]));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	else if (m_Type == DEPTH_CUBE)
	{
		m_TexIDs.push_back(0);
		GLCall(glGenTextures(1, &m_TexIDs[0]));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_TexIDs[0]));
		for (unsigned int i = 0; i < 6; i++)
		{
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		}
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_TexIDs[0], 0));
		// Generate texture handle
		m_Handles.push_back(0);
		m_Handles[0] = glGetTextureHandleARB(m_TexIDs[0]);
		GLCall(glMakeTextureHandleResidentARB(m_Handles[0]));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}
	else if (m_Type == SSAO)
	{
		m_TexIDs.push_back(0);
		GLCall(glGenTextures(1, &m_TexIDs[0]));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexIDs[0], 0));
		m_Handles.push_back(0);
		m_Handles[0] = glGetTextureHandleARB(m_TexIDs[0]);
		GLCall(glMakeTextureHandleResidentARB(m_Handles[0]));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

bool GLFrameBuffer::IsInitialized()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		spdlog::error("Failed to intialize Frame Buffer! RendererID: " + std::to_string(m_RendererID));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		return false;
	}
	return true;
}

void GLFrameBuffer::Bind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void GLFrameBuffer::Unbind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void GLFrameBuffer::BindTex(unsigned int slot) const
{
	switch (m_Type)
	{
	case FRAME:
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]));
		break;
	case DEPTH_MAP:
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]));
		break;
	case DEPTH_CUBE:
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_TexIDs[0]));
		break;
	case G_BUFFER:
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]));
		GLCall(glActiveTexture(GL_TEXTURE0 + slot + 1));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[1]));
		GLCall(glActiveTexture(GL_TEXTURE0 + slot + 2));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[2]));
		break;
	case OIT:
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]));
		GLCall(glActiveTexture(GL_TEXTURE0 + slot + 1));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[1]));
		break;
	case SSAO:
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]));
		break;
	}
}

void GLFrameBuffer::UnbindTex() const
{
	switch (m_Type)
	{
	case FRAME:
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		break;
	case DEPTH_MAP:
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		break;
	case DEPTH_CUBE:
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
		break;
	case G_BUFFER:
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		break;
	case OIT:
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		break;
	case SSAO:
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		break;
	}
}

void GLFrameBuffer::ChangeResolution()
{
	int width = (int)(core::SETTINGS.Width * core::SETTINGS.Resolution);
	int height = (int)(core::SETTINGS.Height * core::SETTINGS.Resolution);
	m_TexSize = glm::vec2(width, height);
	switch (m_Type)
	{
	case FRAME:
		// Render buffer
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		// Color buffer
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		// Depth buffer
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[1]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		break;
	}
}

void GLFrameBuffer::ChangeShadowRes(unsigned int width, unsigned int height)
{
	GLCall(glDeleteTextures(1, &m_TexIDs[0]));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	GLCall(glGenTextures(1, &m_TexIDs[0]));
	m_TexSize = glm::vec2(width, height);
	if (m_Type == DEPTH_MAP)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_TexIDs[0], 0));
		// Generate texture handle
		m_Handles[0] = glGetTextureHandleARB(m_TexIDs[0]);
		GLCall(glMakeTextureHandleResidentARB(m_Handles[0]));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	else if (m_Type == DEPTH_CUBE)
	{
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_TexIDs[0]));
		for (unsigned int i = 0; i < 6; i++)
		{
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		}
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_TexIDs[0], 0));
		// Generate texture handle
		m_Handles[0] = glGetTextureHandleARB(m_TexIDs[0]);
		GLCall(glMakeTextureHandleResidentARB(m_Handles[0]));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}