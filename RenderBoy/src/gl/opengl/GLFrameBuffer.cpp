#include "GLFrameBuffer.h"

GLFrameBuffer::GLFrameBuffer()
{
	m_RendererID = 0;
	m_RenderBufferID = 0;
	m_TexID = 0;
	m_Handle = 0;
	m_TexWidth = 0;
	m_TexHeight = 0;
}

GLFrameBuffer::~GLFrameBuffer()
{
	glDeleteFramebuffers(1, &m_RendererID);
	glDeleteRenderbuffers(1, &m_RenderBufferID);
	if (m_Handle)
	{
		glMakeTextureHandleNonResidentARB(m_Handle);
	}
	glDeleteTextures(1, &m_TexID);
	m_RendererID = 0;
	m_RenderBufferID = 0;
	m_TexID = 0;
	m_Handle = 0;
	m_TexWidth = 0;
	m_TexHeight = 0;
}

void GLFrameBuffer::Init(FBType type)
{
	int width = (int)(core::SETTINGS.Width * core::SETTINGS.Resolution);
	int height = (int)(core::SETTINGS.Height * core::SETTINGS.Resolution);
	int msaa;
	switch (core::SETTINGS.AA)
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
		GLCall(glDeleteTextures(1, &m_TexID));
	}
	m_TexWidth = width;
	m_TexHeight = height;
	m_Type = type;
	GLCall(glGenFramebuffers(1, &m_RendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	GLCall(glGenTextures(1, &m_TexID));
	if (m_Type == FRAME)
	{
		// Render buffer
		GLCall(glGenRenderbuffers(1, &m_RenderBufferID));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		// Color buffer
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexID, 0));
	}
	else if (m_Type == MSAA)
	{
		// Render buffer
		GLCall(glGenRenderbuffers(1, &m_RenderBufferID));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_DEPTH32F_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		// Color buffer
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TexID));
		GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa, GL_RGBA16F, width, height, GL_TRUE));
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_TexID, 0));
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
		GLCall(glDeleteTextures(1, &m_TexID));
	}
	
	m_Type = type;
	m_TexWidth = width;
	m_TexHeight = height;
	GLCall(glGenFramebuffers(1, &m_RendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	GLCall(glGenTextures(1, &m_TexID));
	if (m_Type == FRAME)
	{
		// Render buffer
		GLCall(glGenRenderbuffers(1, &m_RenderBufferID));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		// Color buffer
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexID, 0));
	}
	else if (m_Type == DEPTH_MAP)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_TexID, 0));
		// Generate texture handle
		m_Handle = glGetTextureHandleARB(m_TexID);
		GLCall(glMakeTextureHandleResidentARB(m_Handle));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	else if (m_Type == DEPTH_CUBE)
	{
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_TexID));
		for (unsigned int i = 0; i < 6; i++)
		{
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		}
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_TexID, 0));
		// Generate texture handle
		m_Handle = glGetTextureHandleARB(m_TexID);
		GLCall(glMakeTextureHandleResidentARB(m_Handle));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
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
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexID));
		break;
	case MSAA:
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TexID));
		break;
	case DEPTH_MAP:
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexID));
		break;
	case DEPTH_CUBE:
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_TexID));
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
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		break;
	case DEPTH_CUBE:
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
		break;
	}
}

void GLFrameBuffer::ChangeResolution()
{
	int width = (int)(core::SETTINGS.Width * core::SETTINGS.Resolution);
	int height = (int)(core::SETTINGS.Height * core::SETTINGS.Resolution);
	int msaa;
	switch (core::SETTINGS.AA)
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
		m_TexWidth = width;
		m_TexHeight = height;
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
		m_TexWidth = width;
		m_TexHeight = height;
		// Render buffer
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID));
		GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_DEPTH24_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		// Color buffer
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TexID));
		GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa, GL_RGBA16F, width, height, GL_TRUE));
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
		break;
	}
}

void GLFrameBuffer::ChangeMSAA()
{
	if (m_Type == MSAA)
	{
		int width = (int)(core::SETTINGS.Width * core::SETTINGS.Resolution);
		int height = (int)(core::SETTINGS.Height * core::SETTINGS.Resolution);
		int msaa;
		switch (core::SETTINGS.AA)
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
		GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_DEPTH32F_STENCIL8, width, height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		// Color buffer
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TexID));
		GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa, GL_RGBA16F, width, height, GL_TRUE));
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
	}
}

void GLFrameBuffer::ChangeShadowRes(unsigned int width, unsigned int height)
{
	GLCall(glDeleteTextures(1, &m_TexID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	GLCall(glGenTextures(1, &m_TexID));
	if (m_Type == DEPTH_MAP)
	{
		m_TexWidth = width;
		m_TexHeight = height;
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_TexID, 0));
		// Generate texture handle
		m_Handle = glGetTextureHandleARB(m_TexID);
		GLCall(glMakeTextureHandleResidentARB(m_Handle));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	else if (m_Type == DEPTH_CUBE)
	{
		m_TexWidth = width;
		m_TexHeight = height;
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_TexID));
		for (unsigned int i = 0; i < 6; i++)
		{
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		}
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_TexID, 0));
		// Generate texture handle
		m_Handle = glGetTextureHandleARB(m_TexID);
		GLCall(glMakeTextureHandleResidentARB(m_Handle));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
