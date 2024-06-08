#SHADER VERTEX
#version 460 core

#define POINT_LIGHT_COUNT
#define SPOT_LIGHT_COUNT
#define DIR_LIGHT_COUNT

layout (location = 0) in vec4 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 7) in mat4 a_ModelMat;

out vec3 v_FragPos;
out vec3 v_Normal;

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;

void main()
{
    v_FragPos = vec3(a_ModelMat * a_Position);
    v_Normal = a_Normal;
    gl_Position = u_ProjMat * u_ViewMat * a_ModelMat * a_Position;
}



#SHADER FRAGMENT
#version 460 core

#define POINT_LIGHT_COUNT
#define SPOT_LIGHT_COUNT
#define DIR_LIGHT_COUNT

struct PointLight
{
    vec3 Position;  
    vec3 Color;
    vec3 ADS;
    vec3 CLQ;
    float Range;
    float Intensity;
    bool LightSwitch;
    bool CastShadow;
    samplerCube ShadowMap;
};

layout(location = 0) out vec4 v_FragColor;

in vec3 v_FragPos;
in vec3 v_Normal;

uniform vec3 u_ViewPos;
uniform PointLight u_PointLight[POINT_LIGHT_COUNT];

const vec4 diffuseColor = vec4(0.78, 0.78, 0.78, 1.0);
const vec4 specularColor = vec4(1.0);

vec4 CalcPointLight(int i, vec3 viewDir);

void main()
{
    vec4 result = vec4(0.0);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);

    // Calculate all lights
    for (int i = 0; i < POINT_LIGHT_COUNT; i++)
    {
        if (u_PointLight[i].LightSwitch)
        {
            result += CalcPointLight(i, viewDir);
        }
    }
    
    v_FragColor = vec4(vec3(result), 1.0);
}

vec4 CalcPointLight(int i, vec3 viewDir)
{
    vec3 lightDir = normalize(u_PointLight[i].Position - v_FragPos);
    vec3 normal = normalize(v_Normal);
    // Diffuse shading
    float diff;
    if (normal == vec3(0.0))
    {
        diff = 1.0;
    }
    else
    {
        diff = max(dot(normal, lightDir), 0.0);
    }
    // Specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec;
    if (normal == vec3(0.0))
    {
        spec = 1.0;
    }
    else
    {
        spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    }
    // attenuation
    float distance = length(u_PointLight[i].Position - v_FragPos);
    float attenuation = u_PointLight[i].Intensity / (u_PointLight[i].CLQ.x + u_PointLight[i].CLQ.y * distance + u_PointLight[i].CLQ.z * distance * distance);    
    // combine results
    vec4 ambient, diffuse, specular;
    ambient  = u_PointLight[i].ADS.x * vec4(u_PointLight[i].Color, 1.0) * diffuseColor;
    diffuse  = u_PointLight[i].ADS.y * vec4(u_PointLight[i].Color, 1.0) * diff * diffuseColor;
    specular = u_PointLight[i].ADS.z * spec * specularColor;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec4 lighting = vec4(1.0);
    // Calculate shadow
    vec3 gridSamplingDisk[20] = vec3[]
    (
        vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
        vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
        vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
        vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
        vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
    );
    float currentDepth = length(v_FragPos - u_PointLight[i].Position);
    float shadow = 0.0;
    float bias = 1.5;
    int samples = 20;
    float viewDistance = length(u_ViewPos - v_FragPos);
    float diskRadius = (1.0 + (viewDistance / u_PointLight[i].Range));
    if (currentDepth < u_PointLight[i].Range && u_PointLight[i].CastShadow)
    {
        for(int i = 0; i < samples; i++)
        {
            float closestDepth = texture(u_PointLight[i].ShadowMap, (v_FragPos - u_PointLight[i].Position) + gridSamplingDisk[i] * diskRadius).r;
            closestDepth *= u_PointLight[i].Range;   // undo mapping [0;1]
            if(currentDepth - bias > closestDepth)
            {
                shadow += 1.0;
            }   
        }
        shadow /= float(samples);
        lighting = ambient + (1.0 - shadow) * (diffuse + specular) * exp(-pow(currentDepth, 2) / (2 * (pow(u_PointLight[i].Range / 2, 2))));
    }
    else
    {
        lighting = ambient + (diffuse + specular) * exp(-pow(currentDepth, 2) / (2 * (pow(u_PointLight[i].Range / 2, 2))));
    }
    return lighting;
}