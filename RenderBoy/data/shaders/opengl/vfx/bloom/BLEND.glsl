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
uniform sampler2D u_BloomTex;
uniform float u_BloomStrength;

void main()
{
    vec3 hdrColor = texture(u_ScreenTex, v_TexCoord).rgb;
    vec3 bloomColor = texture(u_BloomTex, v_TexCoord).rgb;
    vec3 result = mix(hdrColor, bloomColor, u_BloomStrength);
    v_FragColor = vec4(result, 1.0);
}