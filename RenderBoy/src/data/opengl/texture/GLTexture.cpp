#include "GLTexture.h"

GLTexture::GLTexture()
{
    m_RendererID = 0;
    m_Handle = 0;
    m_FilePath = "";
    m_LocalBuffer = nullptr;
    m_Width = 0;
    m_Height = 0;
}

GLTexture::~GLTexture()
{
    if (m_Handle)
    {
        glMakeTextureHandleNonResidentARB(m_Handle);
    }
    glDeleteTextures(1, &m_RendererID);
}

void GLTexture::GenTexture(const std::string filepath)
{
    m_LocalBuffer = FreeImage_Load(FreeImage_GetFileType(filepath.c_str(), 0),filepath.c_str());
    FreeImage_FlipVertical(m_LocalBuffer);
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
    GLCall(glDeleteTextures(1, &m_RendererID));
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    unsigned int bpp = FreeImage_GetBPP(m_LocalBuffer);
    if (bpp == 24)
    {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, m_Width, m_Height, 0, GL_BGR, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(m_LocalBuffer)));
    }
    else if (bpp == 32)
    {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, m_Width, m_Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(m_LocalBuffer)));
    }
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