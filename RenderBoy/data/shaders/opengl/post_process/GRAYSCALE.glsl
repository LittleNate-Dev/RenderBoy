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

uniform sampler2D u_ScreenTex;

void main()
{
    vec3 result = texture(u_ScreenTex, v_TexCoord).rgb;
    float average = 0.2126 * result.r + 0.7152 * result.g + 0.0722 * result.b;
    v_FragColor = vec4(vec3(average), 1.0);
} 