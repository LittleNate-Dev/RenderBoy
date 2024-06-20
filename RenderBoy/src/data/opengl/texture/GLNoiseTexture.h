/*
* Class:
*	GLNoiseTexture
* Description:
*	This class can generate various noise texture for various vfx
*/

#pragma once

#include "core/Core.h"

class GLNoiseTexture
{
private:
    unsigned int m_RendererID;
    GLuint64 m_Handle;
    int m_Width, m_Height;

public:
    GLNoiseTexture();
    ~GLNoiseTexture();

    // Tell opengl to generate a texture
    void GenTexture(const Noise_Tex_Type type);
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    // Get Texture's members
    inline unsigned int GetID() const
    {
        return m_RendererID;
    };
    inline GLuint64 GetHandle() const
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
};

