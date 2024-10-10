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
    m_LocalBuffer = FreeImage_Load(FreeImage_GetFileType(filepath.c_str(), 0), filepath.c_str());
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
    if (m_Handle)
    {
        glMakeTextureHandleNonResidentARB(m_Handle);
    }
    GLCall(glDeleteTextures(1, &m_RendererID));
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    unsigned int bpp = FreeImage_GetBPP(m_LocalBuffer);
    if (bpp == 1 || bpp == 4 || bpp == 8)
    {
        m_LocalBuffer = FreeImage_ConvertTo24Bits(m_LocalBuffer);
    }
    FIBITMAP *alphaChannel = FreeImage_GetChannel(m_LocalBuffer, FICC_ALPHA);
    if (alphaChannel == nullptr)
    {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_SRGB, m_Width, m_Height, 0, GL_BGR, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(m_LocalBuffer)));
    }
    else
    {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_SRGB_ALPHA, m_Width, m_Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(m_LocalBuffer)));
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

void GLTexture::GenTexture(const std::string filepath, bool sRGB)
{
    m_LocalBuffer = FreeImage_Load(FreeImage_GetFileType(filepath.c_str(), 0), filepath.c_str());
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
    if (m_Handle)
    {
        glMakeTextureHandleNonResidentARB(m_Handle);
    }
    GLCall(glDeleteTextures(1, &m_RendererID));
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    unsigned int bpp = FreeImage_GetBPP(m_LocalBuffer);
    if (bpp == 1 || bpp == 4 || bpp == 8)
    {
        m_LocalBuffer = FreeImage_ConvertTo24Bits(m_LocalBuffer);
    }
    FIBITMAP* alphaChannel = FreeImage_GetChannel(m_LocalBuffer, FICC_ALPHA);
    if (alphaChannel == nullptr)
    {
        if (sRGB)
        {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_SRGB, m_Width, m_Height, 0, GL_BGR, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(m_LocalBuffer)));
        }
        else
        {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, m_Width, m_Height, 0, GL_BGR, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(m_LocalBuffer)));
        }
    }
    else
    {
        if (sRGB)
        {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_SRGB_ALPHA, m_Width, m_Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(m_LocalBuffer)));
        }
        else
        {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, m_Width, m_Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(m_LocalBuffer)));
        }
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

void GLTexture::GenTexture(Tex_Type type)
{
    if (m_Handle)
    {
        GLCall(glMakeTextureHandleNonResidentARB(m_Handle));
    }
    GLCall(glDeleteTextures(1, &m_RendererID));
    m_RendererID = 0;
    m_Handle = 0;
    m_Width = 0;
    m_Height = 0;
    if (type == NOISE_SSAO)
    {
        m_Width = 4;
        m_Height = 4;
        std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
        std::default_random_engine generator;
        std::vector<glm::vec3> ssaoNoise;
        for (unsigned int i = 0; i < 16; i++)
        {
            glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
            ssaoNoise.push_back(noise);
        }
        GLCall(glGenTextures(1, &m_RendererID));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        m_Handle = glGetTextureHandleARB(m_RendererID);
        GLCall(glMakeTextureHandleResidentARB(m_Handle));
    }
    else if (type == AL_LTC1)
    {
        m_Width = 64;
        m_Height = 64;
        GLCall(glGenTextures(1, &m_RendererID));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_FLOAT, util::LTC1));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        m_Handle = glGetTextureHandleARB(m_RendererID);
        GLCall(glMakeTextureHandleResidentARB(m_Handle));
    }
    else if (type == AL_LTC2)
    {
        m_Width = 64;
        m_Height = 64;
        GLCall(glGenTextures(1, &m_RendererID));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_FLOAT, util::LTC2));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        m_Handle = glGetTextureHandleARB(m_RendererID);
        GLCall(glMakeTextureHandleResidentARB(m_Handle));
    }
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
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