#SHADER VERTEX
#version 460 core
layout (location = 0) in vec4 a_Position;
layout (location = 2) in vec3 a_Normal;
layout (location = 9) in mat4 a_ModelMat;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 u_ViewMat;

void main()
{
    gl_Position = u_ViewMat * a_ModelMat * a_Position; 
    mat3 normalMatrix = mat3(transpose(inverse(u_ViewMat * a_ModelMat)));
    vs_out.normal = normalize(vec3(vec4(normalMatrix * a_Normal, 0.0)));
}



#SHADER GEOMETRY
#version 460 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

uniform float u_Magnitude;
  
uniform mat4 u_ProjMat;

void GenerateLine(int index)
{
    gl_Position = u_ProjMat * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = u_ProjMat * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * u_Magnitude);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}  



#SHADER FRAGMENT
#version 460 core

out vec4 v_FragColor;

uniform vec3 u_Color;

void main()
{
    v_FragColor = vec4(u_Color, 1.0f);
}