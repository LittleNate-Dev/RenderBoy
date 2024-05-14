#include "GLTexture.h"

GLTexture::GLTexture()
{
    m_RendererID = 0;
    m_Handle = 0;
    m_FilePath = "";
    m_LocalBuffer = nullptr;
    m_Width = 0;
    m_Height = 0;
    m_BPP = 0;
}

GLTexture::~GLTexture()
{
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void GLTexture::GenTexture(const std::string filepath)
{
    m_LocalBuffer = FreeImage_Load(FreeImage_GetFileType(filepath.c_str(), 0),filepath.c_str());
    m_LocalBuffer = FreeImage_ConvertTo32Bits(m_LocalBuffer);
    m_Width = FreeImage_GetWidth(m_LocalBuffer);
    m_Height = FreeImage_GetHeight(m_LocalBuffer);
    // Load texture
    if (!m_LocalBuffer)
    {
        spdlog::warn("Failed to load texture from: " + filepath);
        return;
    }
    m_FilePath = filepath;
    // Generate texture id and handle
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(m_LocalBuffer)));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    // Generate texture handle
    m_Handle = glGetTextureHandleARB(m_RendererID);
    GLCall(glMakeTextureHandleResidentARB(m_Handle));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    if (m_LocalBuffer)
    {
        FreeImage_Unload(m_LocalBuffer);
    }
}

void GLTexture::Bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void GLTexture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}