#SHADER VERTEX
#version 460 core

layout(location = 0) in vec4 a_Position;
layout (location = 6) in mat4 a_ModelMat;

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;

void main()
{
    gl_Position = u_ProjMat * u_ViewMat * a_ModelMat * a_Position;
}



#SHADER FRAGMENT
#version 460 core

layout(location = 0) out vec4 v_FragColor;

void main()
{
    v_FragColor = vec4(0.337, 0.603, 0.247, 1.0);
}