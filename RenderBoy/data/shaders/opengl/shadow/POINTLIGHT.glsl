#SHADER VERTEX
#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 9) in mat4 a_ModelMat;

void main()
{
    gl_Position = a_ModelMat * vec4(a_Position, 1.0);
}



#SHADER GEOMETRY
#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 u_ShadowMat[6];

out vec4 v_FragPos;

void main()
{
    for(int face = 0; face < 6; face++)
    {
        gl_Layer = face;
        for(int i = 0; i < 3; i++)
        {
            v_FragPos = gl_in[i].gl_Position;
            gl_Position = u_ShadowMat[face] * v_FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}



#SHADER FRAGMENT
#version 460 core

in vec4 v_FragPos;

uniform vec3 u_LightPos;
uniform float u_FarPlane;

void main()
{
    float lightDistance = length(v_FragPos.xyz - u_LightPos);
    lightDistance = lightDistance / u_FarPlane;
    gl_FragDepth = lightDistance;
}