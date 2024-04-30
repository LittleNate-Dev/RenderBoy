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

uniform sampler2D u_ScreenTex;
uniform float u_Gamma;
uniform float u_Exposure;

void main()
{
    vec3 hdrColor = texture(u_ScreenTex, v_TexCoord).rgb;
    //vec3 result = vec3(1.0) - exp(-hdrColor * u_Exposure);
    vec3 result = vec3(1.0) - exp(-hdrColor * 1.0);
    result = pow(result, vec3(1.0 / u_Gamma));
    v_FragColor = vec4(result, 1.0);
} 