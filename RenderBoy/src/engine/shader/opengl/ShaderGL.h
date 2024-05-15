/*
* Class:
*   Shader
* Description:
*   Yeah, it's called shader and of course it does all shader things
*/

#pragma once

#include "core/Core.h"

struct ShaderProgramSource
{
    bool HasVertex;
    bool HasGeometry;
    bool HasFragment;
    std::string VertexSource;
    std::string GeometrySource;
    std::string FragmentSource;
};

class ShaderGL
{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    std::unordered_map<std::string, int> m_UniformLocationCache;

    ShaderProgramSource ParseShader(std::string filepath);
    unsigned int CreateShader(const ShaderProgramSource& source);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    int GetUniformLocation(const std::string& name);

public:
    ShaderGL();
    ~ShaderGL();

    bool Init(std::string filepath);
    void Bind() const;
    void Unbind() const;
    inline unsigned int GetID() const
    {
        return m_RendererID;
    };
    // Set uniforms
    void SetUniformHandleARB(std::string name, GLuint64 handle);
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformVec2f(const std::string& name, const glm::vec2& vector);
    void SetUniformVec3f(const std::string& name, const glm::vec3& vector);
    void SetUniformVec4f(const std::string& name, const glm::vec4& vector);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
};