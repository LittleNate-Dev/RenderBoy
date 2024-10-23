/*
* Class:
*	GLTexture
* Description:
*	This class handles anything relate to textures
*/

#pragma once

#include "util/LUT.h"
#include "core/Core.h"

enum Tex_Type
{
    NOISE_SSAO = 0,
    AL_LTC1 = 1,
    AL_LTC2 = 2
};

class GLTexture
{
private:
    unsigned int m_RendererID;
    GLuint64 m_Handle;
    std::string m_FilePath;
    FIBITMAP* m_LocalBuffer;
    int m_Width, m_Height;

public:
    GLTexture();
    ~GLTexture();

    // Tell opengl to generate a texture
    bool GenTexture(const std::string filepath);
    bool GenTexture(const std::string filepath, bool sRGB, bool compress);
    void GenTexture(Tex_Type type);
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    // Get Texture's members
    inline unsigned int GetID() const
    {
        return m_RendererID;
    };
    inline GLuint64 GetHandle()
    {
        return m_Handle;
    };
    inline int GetWidth() const
    {
        return m_Width;
    };
    inline int GetHeight() const
    {
        return m_Height;
    };
    inline std::string GetFilePath() const
    {
        return m_FilePath;
    };
};