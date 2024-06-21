#SHADER VERTEX
#version 460 core
layout (location = 0) in vec3 a_Position;
layout (location = 9) in mat4 a_ModelMat;

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;

void main()
{
    gl_Position = u_ProjMat * u_ViewMat * a_ModelMat * vec4(a_Position, 1.0);
}



#SHADER FRAGMENT
#version 460 core

void main()
{             
    gl_FragDepth = gl_FragCoord.z;
}