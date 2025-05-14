#SHADER VERTEX
#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Normal;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_BiTangent;
layout (location = 5) in vec4 a_TexIndex;
layout (location = 6) in vec3 a_ColorIndex;
layout (location = 7) in vec4 a_AttributeIndex;
layout (location = 8) in vec2 a_NHIndex;
layout (location = 9) in mat4 a_ModelMat;

out vec3 v_FragPos;
out vec2 v_TexCoord;
out vec3 v_Normal;
out mat3 v_TBN;
out vec4 v_TexIndex;
out vec3 v_ColorIndex;
out vec2 v_NHIndex;
out vec4 v_AttributeIndex;

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;

void main()
{
    vec4 viewPos = u_ViewMat * a_ModelMat * vec4(a_Position, 1.0);
    v_FragPos = viewPos.xyz;
    v_TexCoord = a_TexCoord;
    mat3 normalMat = transpose(inverse(mat3(a_ModelMat)));
    v_Normal = normalMat * a_Normal;
    vec3 T = normalize(normalMat * a_Tangent);
    vec3 B = normalize(normalMat * a_BiTangent);
    vec3 N = normalize(v_Normal);
    v_TBN = mat3(T, B, N);
    v_TexIndex = a_TexIndex;
    v_ColorIndex = a_ColorIndex;
    v_AttributeIndex = a_AttributeIndex;
    v_NHIndex = a_NHIndex;

    gl_Position = u_ProjMat * viewPos;
}



#SHADER FRAGMENT
#version 460 core

layout (location = 0) out vec4 g_Position;
layout (location = 1) out vec4 g_Normal;
layout (location = 2) out vec4 g_Depth;
layout (location = 3) out vec4 g_Albedo;

// Any fragment that has a alpha value below this threshold will be discarded
#define ALPHA_THRESHOLD 0.5

in vec3 v_FragPos;
in vec2 v_TexCoord;
in vec3 v_Normal;
in mat3 v_TBN;
in vec4 v_TexIndex;
in vec3 v_ColorIndex;
in vec2 v_NHIndex;
in vec4 v_AttributeIndex;

uniform vec2 u_Plane;
uniform vec3 u_Diffuse[];
uniform float u_Transparent[];
uniform sampler2D u_AlbedoTex[];
uniform sampler2D u_NormalTex[];
uniform mat4 u_ViewMat;

vec3 c_Normal = vec3(0.0);
vec2 c_TexCoord = vec2(0.0);
vec3 c_ColorIndex = vec3(-1.0);
vec4 c_AttributeIndex = vec4(-1.0);
int c_AlbedoTexIndex = -1;
int c_NormalTexIndex = -1;
float c_Alpha = 1.0;
vec3 c_Albedo = vec3(0.0);

float LinearizeDepth(float depth);

void main()
{   
    c_TexCoord = v_TexCoord;
    c_AlbedoTexIndex = int(v_TexIndex.x + 0.1);
    c_ColorIndex = v_ColorIndex + vec3(0.1);
    c_NormalTexIndex = int(v_NHIndex.x + 0.1);
    c_AttributeIndex = v_AttributeIndex + vec4(0.1);
    
    if (v_TexIndex.x < 0)
    {
        c_Alpha = u_Transparent[int(c_AttributeIndex.y)];
    }
    else
    {
        c_Alpha = texture(u_AlbedoTex[c_AlbedoTexIndex], v_TexCoord).a;
    }
    if (c_Alpha < ALPHA_THRESHOLD)
    {
        discard;
    }
    // Normal mapping
    if (v_NHIndex.x < 0)
    {
        c_Normal = normalize(vec4(u_ViewMat * vec4(c_Normal, 0.0)).xyz);
    } 
    else
    {
        c_Normal = texture(u_NormalTex[c_NormalTexIndex], c_TexCoord).rgb;
        c_Normal = normalize(c_Normal * 2.0 - 1.0);
        c_Normal = normalize(v_TBN * c_Normal);
        c_Normal = normalize(vec4(u_ViewMat * vec4(c_Normal, 0.0)).xyz);
    }
    // Albedo
    if (v_TexIndex.x < 0)
    {
        c_Albedo = u_Diffuse[int(c_ColorIndex.y)];
    }
    else
    {
        c_Albedo = texture(u_AlbedoTex[c_AlbedoTexIndex], c_TexCoord).rgb;
    }

    // store the fragment position vector in the first gbuffer texture
    g_Position = vec4(v_FragPos, 1.0);
    // also store the per-fragment normals into the gbuffer
    g_Normal = vec4(normalize(c_Normal), 1.0);
    // and the diffuse per-fragment color
    float depth = LinearizeDepth(gl_FragCoord.z); 
    g_Depth = vec4(vec3(depth), 1.0);
    g_Albedo = vec4(c_Albedo, 1.0);
}

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * u_Plane.x * u_Plane.y) / (u_Plane.y + u_Plane.x - z * (u_Plane.y - u_Plane.x));	
}