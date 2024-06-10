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
    bool SoftShadow;
    samplerCube ShadowMap;
    float Bias;
};

layout(location = 0) out vec4 v_FragColor;

in vec3 v_FragPos;
in vec3 v_Normal;

uniform vec3 u_ViewPos;
uniform PointLight u_PointLight[POINT_LIGHT_COUNT];

const vec4 matDiffuse = vec4(0.78, 0.78, 0.78, 1.0);
const float matShininess = 16.0;
const vec4 matSpecular = vec4(1.0);
vec3 viewDir = vec3(1.0);
vec3 normal = vec3(0.0);

vec4 CalcPointLight(int i);

void main()
{
    vec4 result = vec4(0.0);
    viewDir = normalize(u_ViewPos - v_FragPos);
    normal = normalize(v_Normal);
    // Calculate all lights
    for (int i = 0; i < POINT_LIGHT_COUNT; i++)
    {
        if (u_PointLight[i].LightSwitch)
        {
            result += CalcPointLight(i);
        }
    }
    
    v_FragColor = vec4(vec3(result), 1.0);
}

vec4 CalcPointLight(int i)
{
    vec3 lightDir = normalize(u_PointLight[i].Position - v_FragPos);
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
        spec = pow(max(dot(normal, halfwayDir), 0.0), matShininess);
    }
    // attenuation
    float distance = length(u_PointLight[i].Position - v_FragPos);
    float attenuation = u_PointLight[i].Intensity / (u_PointLight[i].CLQ.x + u_PointLight[i].CLQ.y * distance + u_PointLight[i].CLQ.z * distance * distance);    
    // combine results
    vec4 ambient, diffuse, specular;
    ambient  = u_PointLight[i].ADS.x * vec4(u_PointLight[i].Color, 1.0) * matDiffuse;
    diffuse  = u_PointLight[i].ADS.y * vec4(u_PointLight[i].Color, 1.0) * diff * matDiffuse;
    specular = u_PointLight[i].ADS.z * vec4(u_PointLight[i].Color, 1.0) * spec * matSpecular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec4 lighting = vec4(1.0);
    if (u_PointLight[i].CastShadow)
    {   
        float currentDepth = distance;
        if (currentDepth >= u_PointLight[i].Range)
        {
            lighting = ambient + diffuse + specular;
        }
        else
        {
            // Calculate shadow
            float shadow  = 0.0;
            vec3 fragToLight = v_FragPos - u_PointLight[i].Position;
            if (u_PointLight[i].SoftShadow)
            {
                float samples = 4.0;
                float offset  = 0.1;
                for(float x = -offset; x < offset; x += offset / (samples * 0.5))
                {
                    for(float y = -offset; y < offset; y += offset / (samples * 0.5))
                    {
                        for(float z = -offset; z < offset; z += offset / (samples * 0.5))
                        {
                            float closestDepth = texture(u_PointLight[i].ShadowMap, fragToLight + vec3(x, y, z)).r; 
                            closestDepth *= u_PointLight[i].Range;
                            if(currentDepth - u_PointLight[i].Bias > closestDepth)
                            {
                                shadow += 1.0;
                            }
                        }
                    }
                }
                shadow /= (samples * samples * samples);
            }
            else
            {
                float closestDepth = texture(u_PointLight[i].ShadowMap, fragToLight).r;
                closestDepth *= u_PointLight[i].Range;
                float currentDepth = length(fragToLight);
                shadow = currentDepth -  u_PointLight[i].Bias > closestDepth ? 1.0 : 0.0;        
            }
            lighting = ambient + (1.0 - shadow) * (diffuse + specular); 
        }
    }
    else
    {
        lighting = ambient + diffuse + specular;
    }
     
    return lighting;
}