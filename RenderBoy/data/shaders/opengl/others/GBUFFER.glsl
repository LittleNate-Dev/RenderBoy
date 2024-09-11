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

layout (location = 0) out vec4 g_Position;
layout (location = 1) out vec4 g_Normal;
layout (location = 2) out vec4 g_Depth;

in vec3 v_FragPos;
in vec3 v_Normal;

uniform vec2 u_Plane;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * u_Plane.x * u_Plane.y) / (u_Plane.y + u_Plane.x - z * (u_Plane.y - u_Plane.x));	
}

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    g_Position = vec4(v_FragPos, 1.0);
    //g_Position = vec4(1.0);
    // also store the per-fragment normals into the gbuffer
    g_Normal = vec4(normalize(v_Normal), 1.0);
    // and the diffuse per-fragment color
    float depth = LinearizeDepth(gl_FragCoord.z); 
    g_Depth = vec4(vec3(depth), 1.0);
}