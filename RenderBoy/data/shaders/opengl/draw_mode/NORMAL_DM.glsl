#SHADER VERTEX
#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 2) in vec3 a_Normal;
layout (location = 9) in mat4 a_ModelMat;

out vec3 v_FragPos;
out vec3 v_Normal;

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;

void main()
{
    vec4 viewPos = u_ViewMat * a_ModelMat * vec4(a_Position, 1.0);
    v_FragPos = viewPos.xyz;
    v_Normal = mat3(transpose(inverse(u_ViewMat * a_ModelMat))) * a_Normal;

    gl_Position = u_ProjMat * viewPos;
}



#SHADER FRAGMENT
#version 460 core

layout (location = 0) out vec4 g_Normal;

in vec3 v_FragPos;
in vec3 v_Normal;

void main()
{    
    g_Normal = vec4(normalize(v_Normal), 1.0);
}