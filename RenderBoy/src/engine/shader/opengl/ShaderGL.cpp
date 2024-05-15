#include "ShaderGL.h"

ShaderGL::ShaderGL()
{
	m_RendererID = 0;
}

ShaderGL::~ShaderGL()
{
    GLCall(glDeleteProgram(m_RendererID));
}

bool ShaderGL::Init(std::string filepath)
{
    GLCall(glDeleteProgram(m_RendererID));
    m_FilePath = filepath;
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source);
    if (m_RendererID)
    {
        return true;
    }
    else
    {
        return false;
    }
}

ShaderProgramSource ShaderGL::ParseShader(std::string filepath)
{
    std::ifstream stream(filepath);
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, GEOMETRY = 1, FRAGMENT = 2
    };
    std::string line;
    std::stringstream ss[3];
    ShaderType type = ShaderType::NONE;

    ShaderProgramSource sps;
    sps.HasFragment = false;
    sps.HasGeometry = false;
    sps.HasFragment = false;
    while (getline(stream, line))
    {
        if (line.find("#SHADER") != std::string::npos)
        {
            if (line.find("VERTEX") != std::string::npos)
            {
                sps.HasVertex = true;
                type = ShaderType::VERTEX;
            }
            else if (line.find("GEOMETRY") != std::string::npos)
            {
                sps.HasGeometry = true;
                type = ShaderType::GEOMETRY;
            }
            else if (line.find("FRAGMENT") != std::string::npos)
            {
                sps.HasFragment = true;
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    sps.VertexSource = ss[0].str();
    sps.GeometrySource = ss[1].str();
    sps.FragmentSource = ss[2].str();
    return sps;
}

unsigned int ShaderGL::CreateShader(const ShaderProgramSource& source)
{
    unsigned int program = glCreateProgram();
    unsigned int vs, gs, fs;
    if (source.HasVertex)
    {
        vs = CompileShader(GL_VERTEX_SHADER, source.VertexSource);
        glAttachShader(program, vs);
    }
    if (source.HasGeometry)
    {
        gs = CompileShader(GL_GEOMETRY_SHADER, source.GeometrySource);
        glAttachShader(program, gs);
    }
    if (source.HasFragment)
    {
        fs = CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource);
        glAttachShader(program, fs);
    }
    glLinkProgram(program);
    glValidateProgram(program);

    if (source.HasVertex)
    {
        glDeleteShader(vs);
    }
    if (source.HasGeometry)
    {
        glDeleteShader(gs);
    }
    if (source.HasFragment)
    {
        glDeleteShader(fs);
    }

    return program;
}

unsigned int ShaderGL::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::string errorMsg = "Failed to compile ";
        if (type == GL_VERTEX_SHADER)
        {
            errorMsg += "vertex shader: " + m_FilePath;
        }
        else if (type == GL_GEOMETRY_SHADER)
        {
            errorMsg += "geometry shader: " + m_FilePath;
        }
        else
        {
            errorMsg += "fragment shader: " + m_FilePath;
        }
        spdlog::error(errorMsg);
        spdlog::error(message);
        glDeleteShader(id);
    }

    return id;
}

int ShaderGL::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        spdlog::warn("Warning: Uniform '" + name + "' doesn't exist! Path: " + m_FilePath);
    }
    m_UniformLocationCache[name] = location;
    return location;
}

void ShaderGL::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void ShaderGL::Unbind() const
{
    GLCall(glUseProgram(0));
}

void ShaderGL::SetUniformHandleARB(std::string name, GLuint64 handle)
{
    GLCall(glUniformHandleui64ARB(glGetUniformLocation(m_RendererID, name.c_str()), handle));
}

void ShaderGL::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void ShaderGL::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void ShaderGL::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void ShaderGL::SetUniformVec2f(const std::string& name, const glm::vec2& vector)
{
    GLCall(glUniform2fv(GetUniformLocation(name), 1, &vector[0]));
}

void ShaderGL::SetUniformVec3f(const std::string& name, const glm::vec3& vector)
{
    GLCall(glUniform3fv(GetUniformLocation(name), 1, &vector[0]));
}

void ShaderGL::SetUniformVec4f(const std::string& name, const glm::vec4& vector)
{
    GLCall(glUniform4fv(GetUniformLocation(name), 1, &vector[0]));
}

void ShaderGL::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}