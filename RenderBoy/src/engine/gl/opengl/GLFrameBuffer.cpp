#include "GLFrameBuffer.h"

GLFrameBuffer::GLFrameBuffer()
{
	m_RendererID = 0;
	m_RenderBufferID = 0;
	m_TexID = 0;
}

GLFrameBuffer::~GLFrameBuffer()
{
	GLCall(glDeleteFramebuffers(1, &m_RendererID));
	GLCall(glDeleteRenderbuffers(1, &m_RenderBufferID));
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void GLFrameBuffer::Init(FBType type)
{
	int width = (int)(rbcore::SETTINGS.width * rbcore::SETTINGS.resolution);
	int height = (int)(rbcore::SETTINGS.height * rbcore::SETTINGS.resolution);
	int msaa;
	switch (rbcore::SETTINGS.aa)
	{
	case MSAA4X:
		msaa = 4;
		break;
	case MSAA8X:
		msaa = 8;
		break;
	case MSAA16X:
		msaa = 16;
		break;
	case MSAA32X:
		msaa = 32;
		break;
	default:
		msaa = 4;
		break;
	}

	if (m_RendererID)
	{
		GLCall(glDeleteFramebuffers(1, &m_RendererID));
	}
	if (m_RenderBufferID)
	{
		GLCall(glDeleteRenderbuffers(1, &m_RenderBufferID));
	}
	if (m_TexID)
	{
		GLCall(glDeleteTextures(1, &m_RendererID));
	}

	m_Type = type;
	GLCall(glGenFramebuffers(1, &m_RendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	GLCall(glGenTextures(1, &m_TexID));
	switch (type)
	{
	case FRAME:
		// Render buffer
		GLCall(glGenRenderbuffers(1, &m_RenderBufferID));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		// Color buffer
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexID, 0));
		break;
	case MSAA:
		// Render buffer
		GLCall(glGenRenderbuffers(1, &m_RenderBufferID));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_DEPTH24_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		// Color buffer
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TexID));
		GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa, GL_RGBA, width, height, GL_TRUE));
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_TexID, 0));
		break;
	}
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void GLFrameBuffer::Init(FBType type, unsigned int width, unsigned int height)
{
	if (m_RendererID)
	{
		GLCall(glDeleteFramebuffers(1, &m_RendererID));
	}
	if (m_RenderBufferID)
	{
		GLCall(glDeleteRenderbuffers(1, &m_RenderBufferID));
	}
	if (m_TexID)
	{
		GLCall(glDeleteTextures(1, &m_RendererID));
	}
	
	m_Type = type;
	GLCall(glGenFramebuffers(1, &m_RendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	GLCall(glGenTextures(1, &m_TexID));
	switch (type)
	{
	case DEPTH_MAP:
		break;
	case DEPTH_CUBE:
		break;
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

void GLFrameBuffer::BindTex() const
{
	switch (m_Type)
	{
	case FRAME:
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexID));
		break;
	case MSAA:
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TexID));
		break;
	case DEPTH_MAP:
		break;
	case DEPTH_CUBE:
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
	case MSAA:
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
		break;
	case DEPTH_MAP:
		break;
	case DEPTH_CUBE:
		break;
	}
}

void GLFrameBuffer::ChangeResolution()
{
	int width = (int)(rbcore::SETTINGS.width * rbcore::SETTINGS.resolution);
	int height = (int)(rbcore::SETTINGS.height * rbcore::SETTINGS.resolution);
	int msaa;
	switch (rbcore::SETTINGS.aa)
	{
	case MSAA4X:
		msaa = 4;
		break;
	case MSAA8X:
		msaa = 8;
		break;
	case MSAA16X:
		msaa = 16;
		break;
	case MSAA32X:
		msaa = 16;
		break;
	default:
		msaa = 32;
		break;
	}
	switch (m_Type)
	{
	case FRAME:
		// Render buffer
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		// Color buffer
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		break;
	case MSAA:
		// Render buffer
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_DEPTH24_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		// Color buffer
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TexID));
		GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa, GL_RGBA, width, height, GL_TRUE));
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
		break;
	}
}

void GLFrameBuffer::ChangeMSAA()
{
	if (m_Type == MSAA)
	{
		int width = (int)(rbcore::SETTINGS.width * rbcore::SETTINGS.resolution);
		int height = (int)(rbcore::SETTINGS.height * rbcore::SETTINGS.resolution);
		int msaa;
		switch (rbcore::SETTINGS.aa)
		{
		case MSAA4X:
			msaa = 4;
			break;
		case MSAA8X:
			msaa = 8;
			break;
		case MSAA16X:
			msaa = 16;
			break;
		case MSAA32X:
			msaa = 32;
			break;
		default:
			msaa = 4;
			break;
		}
		// Render buffer
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_DEPTH24_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		// Color buffer
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TexID));
		GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa, GL_RGBA, width, height, GL_TRUE));
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
	}
}

unsigned int GLFrameBuffer::GetID()
{
	return m_RendererID;
}

unsigned int GLFrameBuffer::GetRenderBufferID()
{
	return m_RenderBufferID;
}

unsigned int GLFrameBuffer::GetTexID()	
{
	return m_TexID;
}

FBType GLFrameBuffer::GetType()
{
	return m_Type;
}
