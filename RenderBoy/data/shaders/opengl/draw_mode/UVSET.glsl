#SHADER VERTEX
#version 460 core

layout (location = 0) in vec4 a_Position;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 9) in mat4 a_ModelMat;

out vec2 v_TexCoord;

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = u_ProjMat * u_ViewMat * a_ModelMat * a_Position;
}



#SHADER FRAGMENT
#version 460 core

layout(location = 0) out vec4 v_FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_CheckerMap;

const vec4 faceColor = vec4(0.78, 0.78, 0.78, 1.0);

void main()
{
    v_FragColor = texture(u_CheckerMap, v_TexCoord);
}