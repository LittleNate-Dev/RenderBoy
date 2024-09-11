#SHADER VERTEX
#version 460 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0); 
} 



#SHADER FRAGMENT
#version 460 core
#extension GL_ARB_bindless_texture : require

out float v_FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_SSAO;

void main() 
{
    vec2 texelSize = 1.0 / vec2(textureSize(u_SSAO, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(u_SSAO, v_TexCoord + offset).r;
        }
    }
    v_FragColor = result / (4.0 * 4.0);
}  