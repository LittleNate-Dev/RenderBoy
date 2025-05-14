#SHADER VERTEX
#version 460 core

layout (location = 0) in vec3 a_Position;

out vec3 v_TexCoord;

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;

void main()
{
    v_TexCoord = a_Position;
    vec4 pos = u_ProjMat * u_ViewMat * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;
}  



#SHADER FRAGMENT
#version 460 core

layout (location = 0) out vec4 v_FragColor;

in vec3 v_TexCoord;

uniform bool u_UseTex;
uniform vec3 u_Color;
uniform samplerCube u_Skybox;

void main()
{    
    if (u_UseTex)
    {
        v_FragColor = texture(u_Skybox, v_TexCoord);
    }
    else
    {
        v_FragColor = vec4(u_Color, 1.0);
    }
    
}