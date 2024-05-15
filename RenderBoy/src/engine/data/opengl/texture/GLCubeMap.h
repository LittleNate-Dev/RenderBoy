/*
* Class:
*   GLCubemap
* Description:
*   A cubemap, what else can it be?
*/

#pragma once

#include "core/Core.h"

class GLCubemap
{
private:
    unsigned int m_RendererID;
    GLuint64 m_Handle;
    std::vector<std::string> m_FilePath;
    FIBITMAP* m_LocalBuffer;
    std::vector<int> m_Widths, m_Heights;

public:
    GLCubemap();
    ~GLCubemap();

    // Tell opengl to generate a texture
    bool GenTexture(std::vector<std::string> filepath);
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
    inline std::vector<int> GetWidths() const
    {
        return m_Widths;
    };
    inline std::vector<int> GetHeights() const
    {
        return m_Heights;
    };
    inline std::vector<std::string> GetFilePath() const
    {
        return m_FilePath;
    };
};