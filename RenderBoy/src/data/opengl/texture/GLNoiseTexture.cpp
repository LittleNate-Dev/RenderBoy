#include "GLNoiseTexture.h"

GLNoiseTexture::GLNoiseTexture()
{
    m_RendererID = 0;
    m_Handle = 0;
    m_Width = 0;
    m_Height = 0;
}

GLNoiseTexture::~GLNoiseTexture()
{
    if (m_Handle)
    {
        glMakeTextureHandleNonResidentARB(m_Handle);
    }
    glDeleteTextures(1, &m_RendererID);
}

void GLNoiseTexture::GenTexture(const Noise_Tex_Type type)
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
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
}

void GLNoiseTexture::Bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void GLNoiseTexture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}