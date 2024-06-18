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

out vec4 v_FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_SrcTex;
uniform float u_FilterRadius;

void main()
{
    float x = u_FilterRadius;
    float y = u_FilterRadius;

    vec3 a = texture(u_SrcTex, vec2(v_TexCoord.x - x, v_TexCoord.y + y)).rgb;
    vec3 b = texture(u_SrcTex, vec2(v_TexCoord.x,     v_TexCoord.y + y)).rgb;
    vec3 c = texture(u_SrcTex, vec2(v_TexCoord.x + x, v_TexCoord.y + y)).rgb;

    vec3 d = texture(u_SrcTex, vec2(v_TexCoord.x - x, v_TexCoord.y)).rgb;
    vec3 e = texture(u_SrcTex, vec2(v_TexCoord.x,     v_TexCoord.y)).rgb;
    vec3 f = texture(u_SrcTex, vec2(v_TexCoord.x + x, v_TexCoord.y)).rgb;

    vec3 g = texture(u_SrcTex, vec2(v_TexCoord.x - x, v_TexCoord.y - y)).rgb;
    vec3 h = texture(u_SrcTex, vec2(v_TexCoord.x,     v_TexCoord.y - y)).rgb;
    vec3 i = texture(u_SrcTex, vec2(v_TexCoord.x + x, v_TexCoord.y - y)).rgb;


    vec3 result = e * 4.0;
    result += (b + d + f + h)*2.0;
    result += (a + c + g + i);
    result *= 1.0 / 16.0;
    v_FragColor = vec4(result, 1.0);
}