#SHADER VERTEX
#version 460 core

layout (location = 0) in vec4 a_Position;
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

uniform float u_NearPlane;
uniform float u_FarPlane;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * u_NearPlane * u_FarPlane) / (u_FarPlane + u_NearPlane - z * (u_FarPlane - u_NearPlane));	
}

void main()
{
    // divide by far for demonstration
    float depth = LinearizeDepth(gl_FragCoord.z) / u_FarPlane; 
    v_FragColor = vec4(vec3(depth), 1.0);
}