#include "GLVertexBufferLayout.h"

//public
//Template Push() START
template<>
void GLVertexBufferLayout::Push<float>(unsigned int count)
{
    m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_Stride += count * GLVertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<>
void GLVertexBufferLayout::Push<unsigned int>(unsigned int count)
{
    m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_Stride += count * GLVertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
void GLVertexBufferLayout::Push<int>(unsigned int count)
{
    m_Elements.push_back({ GL_INT, count, GL_FALSE });
    m_Stride += count * GLVertexBufferElement::GetSizeOfType(GL_INT);
}

template<>
void GLVertexBufferLayout::Push<unsigned char>(unsigned int count)
{
    m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_Stride += count * GLVertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}
//Template Push() END