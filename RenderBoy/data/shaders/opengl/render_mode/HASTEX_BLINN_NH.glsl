#SHADER VERTEX
#version 460 core

#define POINT_LIGHT_COUNT
#define SPOT_LIGHT_COUNT
#define DIR_LIGHT_COUNT

struct SpotMat
{
    mat4 ProjMat;
    mat4 ViewMat;
};

struct DirMat
{
    mat4 ProjMat[3];
    mat4 ViewMat[3];
};

struct FragPosDir
{
    vec4 FragPos[3];
};

layout (location = 0) in vec4 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Normal;
layout (location = 5) in vec4 a_TexIndex;
layout (location = 6) in vec3 a_ColorIndex;
layout (location = 7) in vec4 a_AttributeIndex;
layout (location = 9) in mat4 a_ModelMat;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoord;
out vec4 v_TexIndex;
out vec3 v_ColorIndex;
out vec4 v_AttributeIndex;
out vec4 v_FragPosSpot[SPOT_LIGHT_COUNT];
out FragPosDir v_FragPosDir[DIR_LIGHT_COUNT];

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;
uniform SpotMat u_SpotMat[SPOT_LIGHT_COUNT];
uniform DirMat u_DirMat[DIR_LIGHT_COUNT];

void main()
{
    vec4 modelPos = a_ModelMat * a_Position;
    v_FragPos = modelPos.xyz;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    v_ColorIndex = a_ColorIndex;
    v_AttributeIndex = a_AttributeIndex;
    if (a_Normal == vec3(0.0))
    {
        v_Normal = vec3(0.0);
    }
    else
    {
        v_Normal = mat3(transpose(inverse(a_ModelMat))) * a_Normal;
    }
    for (int i = 0; i < SPOT_LIGHT_COUNT; i++)
    {
        v_FragPosSpot[i] = u_SpotMat[i].ProjMat * u_SpotMat[i].ViewMat * modelPos;
    }
    for (int i = 0; i < DIR_LIGHT_COUNT; i++)
    {
        v_FragPosDir[i].FragPos[0] = u_DirMat[i].ProjMat[0] * u_DirMat[i].ViewMat[0] * modelPos;
        v_FragPosDir[i].FragPos[1] = u_DirMat[i].ProjMat[1] * u_DirMat[i].ViewMat[1] * modelPos;
        v_FragPosDir[i].FragPos[2] = u_DirMat[i].ProjMat[2] * u_DirMat[i].ViewMat[2] * modelPos;
    }
    gl_Position = u_ProjMat * u_ViewMat * modelPos;
}



#SHADER FRAGMENT
#version 460 core

#define POINT_LIGHT_COUNT
#define SPOT_LIGHT_COUNT
#define DIR_LIGHT_COUNT

struct FragPosDir
{
    vec4 FragPos[3];
};

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
    float FarPlane;
    float SoftDegree;
};

struct SpotLight
{
    vec3 Position;  
    vec3 Direction;
    float Angle;
    float DimAngle;
    vec3 Color;
    vec3 ADS;
    vec3 CLQ;
    float Range;
    float Intensity;
    bool LightSwitch;
    bool CastShadow;
    bool SoftShadow;
    sampler2D ShadowMap;
    vec2 Bias;
    float FarPlane;
    float SoftDegree;
};

struct DirLight
{
    float Intensity;
    vec3 Direction;
    vec3 Color;
    sampler2D ShadowMap[3];
    vec3 ADS;
    bool LightSwitch;
    bool CastShadow;
    vec3 Bias;
    bool SoftShadow;
    float SoftDegree;
};

layout(location = 0) out vec4 v_FragColor;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec4 v_TexIndex;
in vec3 v_ColorIndex;
in vec4 v_AttributeIndex;
in vec4 v_FragPosSpot[SPOT_LIGHT_COUNT];
in FragPosDir v_FragPosDir[DIR_LIGHT_COUNT];

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;
uniform vec3 u_ViewPos;
uniform PointLight u_PointLight[POINT_LIGHT_COUNT];
uniform SpotLight u_SpotLight[SPOT_LIGHT_COUNT];
uniform DirLight u_DirLight[DIR_LIGHT_COUNT];
uniform bool u_SSAO;
uniform sampler2D u_SSAOTex;

// Material
uniform vec3 u_Diffuse[];
uniform vec3 u_Specular[];
uniform float u_Transparent[];
uniform sampler2D u_AlbedoTex[];
uniform sampler2D u_SpecularTex[];
uniform sampler2D u_NormalTex[];
uniform sampler2D u_BumpTex[];
uniform sampler2D u_DisplacementTex[];

const float c_Shininess = 32.0f;

vec3 viewDir = vec3(1.0);
vec3 normal = vec3(0.0);
float ssao = 1.0;
vec3 colorIndex = vec3(-1.0);
vec4 attributeIndex = vec4(-1.0);
int albedoTexIndex = -1;
int specularTexIndex = -1;

vec3 CalcPointLight(int i);
vec3 CalcSpotLight(int i);
vec3 CalcDirLight(int i);

void main()
{
    albedoTexIndex = int(v_TexIndex.x + 0.1);
    specularTexIndex = int(v_TexIndex.y + 0.1);
    colorIndex = v_ColorIndex + vec3(0.1);
    attributeIndex = v_AttributeIndex + vec4(0.1);
    vec3 result = vec3(0.0);
    viewDir = normalize(u_ViewPos - v_FragPos);
    normal = normalize(v_Normal);
    if (v_TexIndex.x < 0)
    {
        if (u_Transparent[int(attributeIndex.y)] < 0.1)
        {
            discard;
        }
    }
    else if (texture(u_AlbedoTex[albedoTexIndex], v_TexCoord).a < 0.1)
    {
        discard;
    }
    // SSAO
    if (u_SSAO)
    {
        vec4 screenCoord = u_ProjMat * u_ViewMat * vec4(v_FragPos, 1.0);
        vec2 aoCoord = screenCoord.xy / screenCoord.w;
        aoCoord = aoCoord * 0.5 + 0.5;
        ssao = texture(u_SSAOTex, aoCoord).r;
    }
    // Calculate all lights
    for (int i = 0; i < POINT_LIGHT_COUNT; i++)
    {
        if (u_PointLight[i].LightSwitch)
        {
            result += CalcPointLight(i);
        }
    }
    for (int i = 0; i < SPOT_LIGHT_COUNT; i++)
    {
        if (u_SpotLight[i].LightSwitch)
        {
            result += CalcSpotLight(i);
        }
    }
    for (int i = 0; i < DIR_LIGHT_COUNT; i++)
    {
        if (u_DirLight[i].LightSwitch)
        {
            result += CalcDirLight(i);
        }
    }
    if (v_TexIndex.x < 0)
    {
        v_FragColor = vec4(result, u_Transparent[int(attributeIndex.y)]);
    }
    else
    {
        v_FragColor = vec4(result, texture(u_AlbedoTex[albedoTexIndex], v_TexCoord).a);
    }
}

vec3 CalcPointLight(int i)
{
    vec3 ambient, diffuse, specular;
    vec3 lightDir = normalize(u_PointLight[i].Position - v_FragPos);
    // Attenuation
    float distance = length(u_PointLight[i].Position - v_FragPos);
    float attenuation = u_PointLight[i].Intensity / (u_PointLight[i].CLQ.x + u_PointLight[i].CLQ.y * distance + u_PointLight[i].CLQ.z * distance * distance); 
    if (v_Normal == vec3(0.0))
    {
        if (v_TexIndex.x < 0)
        {
            ambient  = u_PointLight[i].ADS.x * u_PointLight[i].Color * u_Diffuse[int(colorIndex.y)] * ssao;
            diffuse  = u_PointLight[i].ADS.y * u_PointLight[i].Color * u_Diffuse[int(colorIndex.y)];
        }
        else
        {
            ambient  = u_PointLight[i].ADS.x * u_PointLight[i].Color * texture(u_AlbedoTex[albedoTexIndex], v_TexCoord).rgb * ssao;
            diffuse  = u_PointLight[i].ADS.y * u_PointLight[i].Color * texture(u_AlbedoTex[albedoTexIndex], v_TexCoord).rgb;
        }
        specular = vec3(0.0);
    }
    else
    {
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), c_Shininess);
        if (v_TexIndex.x < 0)
        {
            ambient  = u_PointLight[i].ADS.x * u_PointLight[i].Color * u_Diffuse[int(colorIndex.y)] * ssao;
            diffuse  = u_PointLight[i].ADS.y * u_PointLight[i].Color * diff * u_Diffuse[int(colorIndex.y)];
        }
        else
        {
            ambient  = u_PointLight[i].ADS.x * u_PointLight[i].Color * texture(u_AlbedoTex[albedoTexIndex], v_TexCoord).rgb * ssao;
            diffuse  = u_PointLight[i].ADS.y * u_PointLight[i].Color * diff * texture(u_AlbedoTex[albedoTexIndex], v_TexCoord).rgb;
            
        }
        if (v_TexIndex.y < 0)
        {
            specular = u_PointLight[i].ADS.z * u_PointLight[i].Color * spec;
        }
        else
        {
            specular = u_PointLight[i].ADS.z * u_PointLight[i].Color * spec * texture(u_SpecularTex[specularTexIndex], v_TexCoord).r;
        }
    }
    // combine results
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 lighting = vec3(1.0);
    if (u_PointLight[i].CastShadow)
    {   
        float currentDepth = distance;
        vec3 fragToLight = v_FragPos - u_PointLight[i].Position;
        if (currentDepth >= u_PointLight[i].FarPlane && texture(u_PointLight[i].ShadowMap, fragToLight).r == 1.0)
        {
            lighting = ambient + diffuse + specular;
        }
        else
        {
            // Calculate shadow
            float shadow  = 0.0;
            if (u_PointLight[i].SoftShadow)
            {
                float samples = 4.0;
                float offset  = u_PointLight[i].SoftDegree;
                for(float x = -offset; x < offset; x += offset / (samples * 0.5))
                {
                    for(float y = -offset; y < offset; y += offset / (samples * 0.5))
                    {
                        for(float z = -offset; z < offset; z += offset / (samples * 0.5))
                        {
                            float closestDepth = texture(u_PointLight[i].ShadowMap, fragToLight + vec3(x, y, z)).r; 
                            closestDepth *= u_PointLight[i].FarPlane;
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
                closestDepth *= u_PointLight[i].FarPlane;
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

vec3 CalcSpotLight(int i)
{
    vec3 ambient, diffuse, specular;
    vec3 lightDir = normalize(u_SpotLight[i].Position - v_FragPos);
    // Attenuation
    float distance = length(u_SpotLight[i].Position - v_FragPos);
    float attenuation = u_SpotLight[i].Intensity / (u_SpotLight[i].CLQ.x + u_SpotLight[i].CLQ.y * distance + u_SpotLight[i].CLQ.z * (distance * distance)); 
    // dim out the light
    float theta = dot(-lightDir, normalize(u_SpotLight[i].Direction));
    float epsilon = cos(u_SpotLight[i].Angle * 0.5) - cos(u_SpotLight[i].Angle * 0.5 + u_SpotLight[i].DimAngle);
    float intensity = clamp((theta - cos(u_SpotLight[i].Angle * 0.5 + u_SpotLight[i].DimAngle)) / epsilon, 0.0, 1.0);
    if (v_Normal == vec3(0.0))
    {
        if (v_TexIndex.x < 0)
        {
            ambient  = u_SpotLight[i].ADS.x * u_SpotLight[i].Color * u_Diffuse[int(colorIndex.y)] * ssao;
            diffuse  = u_SpotLight[i].ADS.y * u_SpotLight[i].Color * u_Diffuse[int(colorIndex.y)];
        }
        else
        {
            ambient  = u_SpotLight[i].ADS.x * u_SpotLight[i].Color * texture(u_AlbedoTex[albedoTexIndex], v_TexCoord).rgb * ssao;
            diffuse  = u_SpotLight[i].ADS.y * u_SpotLight[i].Color * texture(u_AlbedoTex[albedoTexIndex], v_TexCoord).rgb;
        }
        specular = vec3(0.0);
    }
    else
    {
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), c_Shininess);
        if (v_TexIndex.x < 0)
        {
            ambient  = u_SpotLight[i].ADS.x * u_SpotLight[i].Color * u_Diffuse[int(colorIndex.y)] * ssao;
            diffuse  = u_SpotLight[i].ADS.y * u_SpotLight[i].Color * diff * u_Diffuse[int(colorIndex.y)];
        }
        else
        {
            ambient  = u_SpotLight[i].ADS.x * u_SpotLight[i].Color * texture(u_AlbedoTex[albedoTexIndex], v_TexCoord).rgb * ssao;
            diffuse  = u_SpotLight[i].ADS.y * u_SpotLight[i].Color * diff * texture(u_AlbedoTex[albedoTexIndex], v_TexCoord).rgb;
        }
        if (v_TexIndex.y < 0)
        {
            specular = u_SpotLight[i].ADS.z * u_SpotLight[i].Color * spec;
        }
        else
        {
            specular = u_SpotLight[i].ADS.z * u_SpotLight[i].Color * spec * texture(u_SpecularTex[specularTexIndex], v_TexCoord).r;
        }
        
    }
    ambient *= attenuation;
    diffuse *= intensity * attenuation;
    specular *= intensity * attenuation;

    vec3 lighting = vec3(1.0);
    if (u_SpotLight[i].CastShadow)
    {
        float bias = max(u_SpotLight[i].Bias.x * (1.0 - dot(normal, lightDir)), u_SpotLight[i].Bias.y);
        vec3 ndc = v_FragPosSpot[i].xyz / v_FragPosSpot[i].w;
        if (ndc.x < 1.0 && ndc.x > -1.0 && ndc.y < 1.0 && ndc.y > -1.0 && ndc.z > -1.0)
        {
            // Inside light's frustum
            lighting = vec3(1.0);
            vec2 shadowTex = ndc.xy;
            shadowTex = shadowTex * 0.5 + 0.5;
            float closestDepth = texture(u_SpotLight[i].ShadowMap, shadowTex).r;
            float currentDepth = ndc.z * 0.5 + 0.5;
            if (closestDepth == 1.0)
            {
                lighting = ambient + diffuse + specular;
            }
            else
            {
                closestDepth *= u_SpotLight[i].FarPlane;
                currentDepth *= u_SpotLight[i].FarPlane;
                if (currentDepth - bias > closestDepth)
                {
                    if (u_SpotLight[i].SoftShadow)
                    {
                        // PCF
                        float shadow = 0.0;
                        vec2 texelSize = 1.0 / textureSize(u_SpotLight[i].ShadowMap, 0);
                        float offset = u_SpotLight[i].SoftDegree;
                        for(float x = -offset; x <= offset; ++x)
                        {
                            for(float y = -offset; y <= offset; ++y)
                            {
                                float pcfDepth = texture(u_SpotLight[i].ShadowMap, shadowTex.xy + vec2(x, y) * texelSize).r; 
                                pcfDepth *= u_SpotLight[i].FarPlane;
                                shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
                            }    
                        }
                        shadow /= pow(offset * 2.0 + 1.0, 2.0);
                        lighting = ambient + (1.0 - shadow) * (diffuse + specular);
                    }
                    else
                    {
                        lighting = ambient;
                    }
                }
                else
                {
                    lighting = ambient + diffuse + specular;
                }
            }
        }
        else
        {
            lighting = ambient + diffuse + specular;
        }
    }
    else
    {
        lighting = ambient + diffuse + specular;
    }
    return lighting;
}

vec3 CalcDirLight(int i)
{
    vec3 ambient, diffuse, specular;
    vec3 lightDir = u_DirLight[i].Direction;
    // Attenuation
    float attenuation = u_DirLight[i].Intensity;  
    if (v_Normal == vec3(0.0))
    {
        if (v_TexIndex.x < 0)
        {
            ambient  = u_DirLight[i].ADS.x * u_DirLight[i].Color * u_Diffuse[int(colorIndex.y)] * ssao;
            diffuse  = u_DirLight[i].ADS.y * u_DirLight[i].Color * u_Diffuse[int(colorIndex.y)];
        }
        else
        {
            ambient  = u_DirLight[i].ADS.x * u_DirLight[i].Color * texture(u_AlbedoTex[albedoTexIndex], v_TexCoord).rgb * ssao;
            diffuse  = u_DirLight[i].ADS.y * u_DirLight[i].Color * texture(u_AlbedoTex[albedoTexIndex], v_TexCoord).rgb;
        }
        specular = vec3(0.0);
    }
    else
    {
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), c_Shininess);
        if (v_TexIndex.x < 0)
        {
            ambient  = u_DirLight[i].ADS.x * u_DirLight[i].Color * u_Diffuse[int(colorIndex.y)] * ssao;
            diffuse  = u_DirLight[i].ADS.y * u_DirLight[i].Color * diff * u_Diffuse[int(colorIndex.y)];
        }
        else
        {
            ambient  = u_DirLight[i].ADS.x * u_DirLight[i].Color * texture(u_AlbedoTex[albedoTexIndex], v_TexCoord).rgb * ssao;
            diffuse  = u_DirLight[i].ADS.y * u_DirLight[i].Color * diff * texture(u_AlbedoTex[albedoTexIndex], v_TexCoord).rgb;
        }
        if (v_TexIndex.y < 0)
        {
            specular = u_DirLight[i].ADS.z * u_DirLight[i].Color * spec;
        }
        else
        {
            specular = u_DirLight[i].ADS.z * u_DirLight[i].Color * spec * texture(u_SpecularTex[specularTexIndex], v_TexCoord).r;
        }
    }
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 lighting = vec3(0.0);
    if (u_DirLight[i].CastShadow)
    {
        int level = 0;
        vec3 ndc = vec3(0.0);
        for (level = 0; level < 3; level++)
        {
            ndc = v_FragPosDir[i].FragPos[level].xyz / v_FragPosDir[i].FragPos[level].w;
            if (ndc.x < 1.0 && ndc.x > -1.0 && ndc.y < 1.0 && ndc.y > -1.0 && ndc.z > -1.0 && ndc.z < 1.0)
            {
                break;
            }
        }
        vec2 shadowTex = ndc.xy;
        shadowTex = shadowTex * 0.5 + 0.5;
        float closestDepth = texture(u_DirLight[i].ShadowMap[level], shadowTex).r;
        float currentDepth = ndc.z * 0.5 + 0.5;
        if (closestDepth == 1.0)
        {
            lighting = ambient + diffuse + specular;
        }
        else
        {
            if (currentDepth - u_DirLight[i].Bias[level] > closestDepth)
            {
                if (u_DirLight[i].SoftShadow)
                {
                    // PCF
                    float shadow = 0.0;
                    vec2 texelSize = 1.0 / textureSize(u_DirLight[i].ShadowMap[level], 0);
                    float offset = u_DirLight[i].SoftDegree;
                    for(float x = -offset; x <= offset; ++x)
                    {
                        for(float y = -offset; y <= offset; ++y)
                        {
                            float pcfDepth = texture(u_DirLight[i].ShadowMap[level], shadowTex.xy + vec2(x, y) * texelSize).r; 
                            shadow += currentDepth - u_DirLight[i].Bias[level] > pcfDepth  ? 1.0 : 0.0;        
                        }    
                    }
                    shadow /= pow(offset * 2.0 + 1.0, 2.0);
                    lighting = ambient + (1.0 - shadow) * (diffuse + specular);
                }
                else
                {
                    lighting = ambient;
                }
            }
            else
            {
                lighting = ambient + diffuse + specular;
            }
        }
    }
    else
    {
        lighting = ambient + diffuse + specular;
    }
    return lighting;
}