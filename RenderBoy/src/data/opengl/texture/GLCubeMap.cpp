#include "GLCubeMap.h"

GLCubeMap::GLCubeMap()
{
	m_RendererID = 0;
	m_Handle = 0;
    m_LocalBuffer = nullptr;
}

GLCubeMap::~GLCubeMap()
{
    if (m_Handle)
    {
        glMakeTextureHandleNonResidentARB(m_Handle);
    }
	glDeleteTextures(1, &m_RendererID);
}

bool GLCubeMap::GenTexture(std::vector<std::string> filepath)
{
    std::vector<int>().swap(m_Widths);
    std::vector<int>().swap(m_Heights);
    if (m_Handle)
    {
        glMakeTextureHandleNonResidentARB(m_Handle);
    }
    GLCall(glDeleteTextures(1, &m_RendererID));
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
    for (unsigned int i = 0; i < filepath.size(); i++)
    {
        m_LocalBuffer = FreeImage_Load(FreeImage_GetFileType(filepath[i].c_str(), 0), filepath[i].c_str());
        FreeImage_FlipVertical(m_LocalBuffer);
        m_Widths.push_back(FreeImage_GetWidth(m_LocalBuffer));
        m_Heights.push_back(FreeImage_GetHeight(m_LocalBuffer));
        if (m_LocalBuffer)
        {
            unsigned int bpp = FreeImage_GetBPP(m_LocalBuffer);
            if (bpp == 24)
            {
                GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_COMPRESSED_SRGB, m_Widths[i], m_Heights[i], 0, GL_BGR, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(m_LocalBuffer)));
            }
            else if (bpp == 32)
            {
                GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_COMPRESSED_SRGB_ALPHA, m_Widths[i], m_Heights[i], 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(m_LocalBuffer)));
            }
            FreeImage_Unload(m_LocalBuffer);
        }
        else
        {
            std::string errorMsg = "Cubemap texture failed to load at path: " + filepath[i];
            spdlog::error(errorMsg);
            return false;
        }
    }
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    m_Handle = glGetTextureHandleARB(m_RendererID);
    GLCall(glMakeTextureHandleResidentARB(m_Handle));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    m_FilePath = filepath;
    return true;
}

void GLCubeMap::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
}

void GLCubeMap::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}