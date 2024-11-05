#SHADER VERTEX
#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Normal;
layout (location = 5) in vec4 a_TexIndex;
layout (location = 7) in vec4 a_AttributeIndex;
layout (location = 9) in mat4 a_ModelMat;

out vec3 v_FragPos;
out vec2 v_TexCoord;
out vec3 v_Normal;
out vec4 v_TexIndex;
out vec4 v_AttributeIndex;

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;

void main()
{
    vec4 viewPos = u_ViewMat * a_ModelMat * vec4(a_Position, 1.0);
    v_FragPos = viewPos.xyz;
    v_TexCoord = a_TexCoord;
    v_Normal = mat3(transpose(inverse(u_ViewMat * a_ModelMat))) * a_Normal;
    v_TexIndex = a_TexIndex;
    v_AttributeIndex = a_AttributeIndex;

    gl_Position = u_ProjMat * viewPos;
}



#SHADER FRAGMENT
#version 460 core

layout (location = 0) out vec4 g_Position;
layout (location = 1) out vec4 g_Normal;
layout (location = 2) out vec4 g_Depth;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec4 v_TexIndex;
in vec4 v_AttributeIndex;

uniform vec2 u_Plane;
uniform float u_Transparent[];
uniform sampler2D u_AlbedoTex[];

vec2 c_TexCoord = vec2(0.0);
vec4 c_AttributeIndex = vec4(-1.0);
int c_AlbedoTexIndex = -1;
float c_Alpha = 1.0;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * u_Plane.x * u_Plane.y) / (u_Plane.y + u_Plane.x - z * (u_Plane.y - u_Plane.x));	
}

void main()
{   
    c_TexCoord = v_TexCoord;
    c_AlbedoTexIndex = int(v_TexIndex.x + 0.1);
    c_AttributeIndex = v_AttributeIndex + vec4(0.1);
    
    if (v_TexIndex.x < 0)
    {
        c_Alpha = u_Transparent[int(c_AttributeIndex.y)];
    }
    else
    {
        c_Alpha = texture(u_AlbedoTex[c_AlbedoTexIndex], v_TexCoord).a;
    }
    if (c_Alpha < 1.0)
    {
        discard;
    }
    // store the fragment position vector in the first gbuffer texture
    g_Position = vec4(v_FragPos, 1.0);
    // also store the per-fragment normals into the gbuffer
    g_Normal = vec4(normalize(v_Normal), 1.0);
    // and the diffuse per-fragment color
    float depth = LinearizeDepth(gl_FragCoord.z); 
    g_Depth = vec4(vec3(depth), 1.0);
}