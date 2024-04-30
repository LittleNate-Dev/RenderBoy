#include "GLVertexArray.h"

GLVertexArray::GLVertexArray()
{
    m_RendererID = 0;
    m_Location = 0;
}

GLVertexArray::~GLVertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void GLVertexArray::GenVertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

//public
void GLVertexArray::AddBuffer(const GLVertexBuffer& vb, const GLVertexBufferLayout& layout)
{
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto element = elements[i];
        GLCall(glEnableVertexAttribArray(m_Location));
        GLCall(glVertexAttribPointer(m_Location, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
        offset += element.count * GLVertexBufferElement::GetSizeOfType(element.type);
        m_Location++;
    }
    vb.Unbind();
    Unbind();
}

void GLVertexArray::AddBuffer(const GLVertexBuffer& vb, const GLVertexBufferLayout& layout, unsigned int divisor)
{
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto element = elements[i];
        GLCall(glEnableVertexAttribArray(m_Location));
        GLCall(glVertexAttribPointer(m_Location, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
        offset += element.count * GLVertexBufferElement::GetSizeOfType(element.type);
        GLCall(glVertexAttribDivisor(m_Location, divisor));
        m_Location++;
    }
    vb.Unbind();
    Unbind();
}

void GLVertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void GLVertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}

unsigned int GLVertexArray::GetID() const
{
    return m_RendererID;
}