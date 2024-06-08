#SHADER VERTEX
#version 460 core

layout (location = 0) in vec4 a_Position;
layout (location = 7) in mat4 a_ModelMat;

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;

void main()
{
    gl_Position = u_ProjMat * u_ViewMat * a_ModelMat * a_Position;
}



#SHADER FRAGMENT
#version 460 core

layout(location = 0) out vec4 v_FragColor;

const vec4 faceColor = vec4(0.78, 0.78, 0.78, 1.0);

void main()
{
    v_FragColor = faceColor;
}