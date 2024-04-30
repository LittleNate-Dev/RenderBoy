/*
* Class:
*   GLVertexBufferLayout
* Description:
*   This class describes the date structure vertex buffer, you need to tell 
*   vertex array the data structure when adding vertex buffer
*/

#pragma once

#include "core/Core.h"

struct GLVertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return 4;
		case GL_UNSIGNED_INT:
			return 4;
		case GL_INT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class GLVertexBufferLayout
{
private:
    std::vector<GLVertexBufferElement> m_Elements;
    unsigned int m_Stride;
    
public:
    GLVertexBufferLayout()
        : m_Stride(0) {}
        
    // Add a new vertex element to the data structure
    template<typename T>
    void Push(unsigned int count);

    inline const std::vector<GLVertexBufferElement> GetElements() const& { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }
};