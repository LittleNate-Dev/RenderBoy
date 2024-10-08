#SHADER VERTEX
#version 460 core

layout(location = 0) in vec4 a_Position;

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ModelMat;

void main()
{
    gl_Position = u_ProjMat * u_ViewMat * u_ModelMat * a_Position;
}



#SHADER FRAGMENT
#version 460 core

layout(location = 0) out vec4 v_FragColor;

uniform vec3 u_Color;
uniform float u_Intensity;

void main()
{
    v_FragColor = vec4(u_Color * u_Intensity, 1.0);
}