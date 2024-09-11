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

layout (location = 0) out vec4 v_FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_Source;
uniform sampler2D u_Depth;
uniform float u_Distance;
uniform float u_Range;
uniform float u_Radius;

void main()
{
    float depth = texture(u_Depth, v_TexCoord).r;
    depth = depth == 0.0 ? 1e20 : depth;
    float coc = (depth - u_Distance) / u_Range;
    coc = clamp(coc, -1, 1) * u_Radius;
    
    v_FragColor = vec4(texture(u_Source, v_TexCoord).rgb, coc);
}