#SHADER VERTEX
#version 460 core

#define POINT_LIGHT_COUNT
#define SPOT_LIGHT_COUNT
#define DIR_LIGHT_COUNT
#define AREA_LIGHT_COUNT

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
#define AREA_LIGHT_COUNT
#define PI 3.14159265359

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

struct AreaLight
{
    int Type;
    vec3 Position;
    vec3 Scale;
    vec3 Points[4];
    float Intensity;
    vec3 Color;
    bool TwoSided;
    bool LightSwitch;
};

layout(location = 0) out vec4 v_Accum;
layout(location = 1) out float v_Reveal;

const float LUT_SIZE  = 64.0; // ltc_texture size 
const float LUT_SCALE = (LUT_SIZE - 1.0)/LUT_SIZE;
const float LUT_BIAS  = 0.5/LUT_SIZE;

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
uniform AreaLight u_AreaLight[AREA_LIGHT_COUNT];
uniform bool u_SSAO;
uniform sampler2D u_SSAOTex;
// LTC used for area light
uniform sampler2D u_LTC1;
uniform sampler2D u_LTC2;

// Material
uniform vec3 u_Diffuse[];
uniform float u_Transparent[];
uniform sampler2D u_AlbedoTex[];
uniform sampler2D u_MetallicTex[];
uniform sampler2D u_RoughnessTex[];
uniform sampler2D u_AoTex[];

uniform bool u_OITPass;

vec3 c_ViewDir = vec3(1.0);
vec3 c_Normal = vec3(0.0);
float c_SSAO = 1.0;
vec3 c_ColorIndex = vec3(-1.0);
vec4 c_AttributeIndex = vec4(-1.0);
int c_AlbedoTexIndex = -1;
int c_MetallicTexIndex = -1;
int c_RoughnessTexIndex = -1;
int c_AoTexIndex = -1;
vec3 c_Albedo = vec3(0.0);
float c_Metallic = 0.0;
float c_Roughness = 1.0;
float c_AO = 1.0;
vec3 c_F0 = vec3(0.04); 
float c_Alpha = 1.0;

vec3 CalcPointLight(int i);
vec3 CalcSpotLight(int i);
vec3 CalcDirLight(int i);
vec3 CalcAreaLight(int i);
vec3 CalcAreaLightRect(int i);
vec3 CalcAreaLightDisk(int i);
vec3 CalcAreaLightCylinder(int i);
// Helper functions
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);
vec3 EvaluateLTCRect(vec3 N, vec3 V, vec3 P, mat3 Minv, vec3 vertex[4], bool twoSided);
vec3 EvaluateLTCDisk(vec3 N, vec3 V, vec3 P, mat3 Minv, vec3 vertex[4], bool twoSided);
vec3 EvaluateLTCCylinder(vec3 N, vec3 V, vec3 P, mat3 Minv, vec3 vertex[4], float radius, bool endCaps);
vec3 IntegrateEdgeVec(vec3 v1, vec3 v2);
float IntegrateEdge(vec3 v1, vec3 v2);
vec3 SolveCubic(vec4 Coefficient);
float Fpo(float d, float l);
float Fwt(float d, float l);
float D(vec3 w, mat3 Minv);

void main()
{
    c_AlbedoTexIndex = int(v_TexIndex.x + 0.1);
    c_MetallicTexIndex = int(v_TexIndex.y + 0.1);
    c_RoughnessTexIndex = int(v_TexIndex.z + 0.1);
    c_AoTexIndex = int(v_TexIndex.w + 0.1);
    c_ColorIndex = v_ColorIndex + vec3(0.1);
    c_AttributeIndex = v_AttributeIndex + vec4(0.1);
    c_ViewDir = normalize(u_ViewPos - v_FragPos);
    c_Normal = normalize(v_Normal);
    if (v_TexIndex.x < 0)
    {
        c_Alpha = u_Transparent[int(c_AttributeIndex.y)];
    }
    else
    {
        c_Alpha = texture(u_AlbedoTex[c_AlbedoTexIndex], v_TexCoord).a;
    }
    if (!u_OITPass && c_Alpha < 1.0)
    {
        discard;
    }
    else if (u_OITPass && (c_Alpha == 1.0 || c_Alpha < 0.05))
    {
        discard;
    }
    // SSAO
    if (u_SSAO)
    {
        vec4 screenCoord = u_ProjMat * u_ViewMat * vec4(v_FragPos, 1.0);
        vec2 aoCoord = screenCoord.xy / screenCoord.w;
        aoCoord = aoCoord * 0.5 + 0.5;
        c_SSAO = texture(u_SSAOTex, aoCoord).r;
    }
    if (v_TexIndex.x < 0)
    {
        c_Albedo = u_Diffuse[int(c_ColorIndex.y)];
    }
    else
    {
        c_Albedo = texture(u_AlbedoTex[c_AlbedoTexIndex], v_TexCoord).rgb;
        c_Metallic = texture(u_MetallicTex[c_MetallicTexIndex], v_TexCoord).b;
        c_Roughness = texture(u_RoughnessTex[c_RoughnessTexIndex], v_TexCoord).r;
    }
    if (v_TexIndex.w >= 0)
    {
        c_AO = texture(u_AoTex[c_AoTexIndex], v_TexCoord).r;
    }
    c_AO *= c_SSAO;
    c_F0 = mix(c_F0, c_Albedo, c_Metallic);
    // Calculate all lights
    vec3 result = vec3(0.0);
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
    for (int i = 0; i < AREA_LIGHT_COUNT; i++)
    {
        if (u_AreaLight[i].LightSwitch)
        {
            result += CalcAreaLight(i);
        }
    }
    vec4 outputData = vec4(result, c_Alpha);
    if (!u_OITPass)
    {
        v_Accum = outputData;
    }
    else
    {
        float weight = clamp(pow(min(1.0, outputData.a * 10.0) + 0.01, 3.0) * 1e8 * 
                         pow(1.0 - gl_FragCoord.z * 0.9, 3.0), 1e-2, 3e3);
        v_Accum = vec4(outputData.rgb * outputData.a, outputData.a) * weight;
        v_Reveal = outputData.a;
    }
}

vec3 CalcPointLight(int i)
{
    vec3 lightDir = normalize(u_PointLight[i].Position - v_FragPos);
    vec3 halfwayDir = normalize(c_ViewDir + lightDir);
    float distance = length(u_PointLight[i].Position - v_FragPos);
    float attenuation = u_PointLight[i].Intensity / (u_PointLight[i].CLQ.x + u_PointLight[i].CLQ.y * distance + u_PointLight[i].CLQ.z * distance * distance);
    vec3 radiance = u_PointLight[i].Color * attenuation;

    float NDF = DistributionGGX(c_Normal, halfwayDir, c_Roughness);   
    float G = GeometrySmith(c_Normal, c_ViewDir, lightDir, c_Roughness);      
    vec3 F = FresnelSchlick(max(dot(halfwayDir, c_ViewDir), 0.0), c_F0);

    vec3 numerator = NDF * G * F; 
    float denominator = 4.0 * max(dot(c_Normal, c_ViewDir), 0.0) * max(dot(c_Normal, lightDir), 0.0) + 0.0001;
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - c_Metallic;	  
    float NdotL = max(dot(c_Normal, lightDir), 0.0);

    vec3 ambient, diffuse, specular;
    ambient  = u_PointLight[i].ADS.x * u_PointLight[i].Color * c_Albedo * c_AO * attenuation;
    diffuse = c_Albedo;
    specular = numerator / denominator;

    vec3 lighting = vec3(1.0);
    if (u_PointLight[i].CastShadow)
    {   
        float currentDepth = distance;
        vec3 fragToLight = v_FragPos - u_PointLight[i].Position;
        if (currentDepth >= u_PointLight[i].FarPlane && texture(u_PointLight[i].ShadowMap, fragToLight).r == 1.0)
        {
            lighting = ambient + (kD * diffuse / PI + specular) * radiance * NdotL;
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
            lighting = ambient + (1.0 - shadow) * (kD * diffuse / PI + specular) * radiance * NdotL;
        }
    }
    else
    {
        lighting = ambient + (kD * diffuse / PI + specular) * radiance * NdotL;
    }
    return lighting;
}

vec3 CalcSpotLight(int i)
{
    vec3 lightDir = normalize(u_SpotLight[i].Position - v_FragPos);
    vec3 halfwayDir = normalize(c_ViewDir + lightDir);
    float distance = length(u_SpotLight[i].Position - v_FragPos);
    float attenuation = u_SpotLight[i].Intensity / (u_SpotLight[i].CLQ.x + u_SpotLight[i].CLQ.y * distance + u_SpotLight[i].CLQ.z * distance * distance);
    // dim out the light
    float theta = dot(-lightDir, normalize(u_SpotLight[i].Direction));
    float epsilon = cos(u_SpotLight[i].Angle * 0.5) - cos(u_SpotLight[i].Angle * 0.5 + u_SpotLight[i].DimAngle);
    float intensity = clamp((theta - cos(u_SpotLight[i].Angle * 0.5 + u_SpotLight[i].DimAngle)) / epsilon, 0.0, 1.0);
    vec3 radiance = u_SpotLight[i].Color * attenuation * intensity;

    float NDF = DistributionGGX(c_Normal, halfwayDir, c_Roughness);   
    float G = GeometrySmith(c_Normal, c_ViewDir, lightDir, c_Roughness);      
    vec3 F = FresnelSchlick(max(dot(halfwayDir, c_ViewDir), 0.0), c_F0);

    vec3 numerator = NDF * G * F; 
    float denominator = 4.0 * max(dot(c_Normal, c_ViewDir), 0.0) * max(dot(c_Normal, lightDir), 0.0) + 0.0001;
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - c_Metallic;	  
    float NdotL = max(dot(c_Normal, lightDir), 0.0);

    vec3 ambient, diffuse, specular;
    ambient  = u_SpotLight[i].ADS.x * u_SpotLight[i].Color * c_Albedo * c_AO * attenuation;
    diffuse = c_Albedo;
    specular = numerator / denominator;

    vec3 lighting = vec3(1.0);
    if (u_SpotLight[i].CastShadow)
    {
        float bias = max(u_SpotLight[i].Bias.x * (1.0 - dot(c_Normal, lightDir)), u_SpotLight[i].Bias.y);
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
                lighting = ambient + (kD * diffuse / PI + specular) * radiance * NdotL;
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
                        lighting = ambient + (1.0 - shadow) * (kD * diffuse / PI + specular) * radiance * NdotL;
                    }
                    else
                    {
                        lighting = ambient;
                    }
                }
                else
                {
                    lighting = ambient + (kD * diffuse / PI + specular) * radiance * NdotL;
                }
            }
        }
        else
        {
            lighting = ambient + (kD * diffuse / PI + specular) * radiance * NdotL;
        }
    }
    else
    {
        lighting = ambient + (kD * diffuse / PI + specular) * radiance * NdotL;
    }
    return lighting;
}

vec3 CalcDirLight(int i)
{
    
    vec3 lightDir = u_DirLight[i].Direction;
    vec3 halfwayDir = normalize(c_ViewDir + lightDir);
    float attenuation = u_DirLight[i].Intensity;
    vec3 radiance = u_DirLight[i].Color * attenuation;

    float NDF = DistributionGGX(c_Normal, halfwayDir, c_Roughness);   
    float G = GeometrySmith(c_Normal, c_ViewDir, lightDir, c_Roughness);      
    vec3 F = FresnelSchlick(max(dot(halfwayDir, c_ViewDir), 0.0), c_F0);

    vec3 numerator = NDF * G * F; 
    float denominator = 4.0 * max(dot(c_Normal, c_ViewDir), 0.0) * max(dot(c_Normal, lightDir), 0.0) + 0.0001;
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - c_Metallic;	  
    float NdotL = max(dot(c_Normal, lightDir), 0.0);

    vec3 ambient, diffuse, specular;
    ambient  = u_DirLight[i].ADS.x * u_DirLight[i].Color * c_Albedo * c_AO * attenuation;
    diffuse = c_Albedo;
    specular = numerator / denominator;

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
            lighting = ambient + (kD * diffuse / PI + specular) * radiance * NdotL;
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
                    lighting = ambient + (1.0 - shadow) * (kD * diffuse / PI + specular) * radiance * NdotL;
                }
                else
                {
                    lighting = ambient;
                }
            }
            else
            {
                lighting = ambient + (kD * diffuse / PI + specular) * radiance * NdotL;
            }
        }
    }
    else
    {
        lighting = ambient + (kD * diffuse / PI + specular) * radiance * NdotL;
    }
    return lighting;
}

vec3 CalcAreaLight(int i)
{
    vec3 lighting = vec3(0.0);
    switch (u_AreaLight[i].Type)
    {
    case 0:
        lighting = CalcAreaLightRect(i);
        break;
    case 1:
        lighting = CalcAreaLightCylinder(i);
        break;
    case 2:
        lighting = CalcAreaLightDisk(i);
        break;
    }    
    return lighting;
}

vec3 CalcAreaLightRect(int i)
{
    vec3 lighting = vec3(0.0);
    float dotNV = clamp(dot(c_Normal, c_ViewDir), 0.0, 1.0);
    // use roughness and sqrt(1-cos_theta) to sample M_texture
    vec2 uv = vec2(c_Roughness, sqrt(1.0 - dotNV));
    uv = uv * LUT_SCALE + LUT_BIAS;
    // get 4 parameters for inverse_M
    vec4 t1 = texture(u_LTC1, uv);
    // Get 2 parameters for Fresnel calculation
    vec4 t2 = texture(u_LTC2, uv);
    mat3 Minv = mat3(
        vec3(t1.x, 0.0, t1.y),
        vec3(0.0,  1.0,  0.0),
        vec3(t1.z, 0.0, t1.w)
    );
    // Evaluate LTC shading
    vec3 diffuse, specular;
    diffuse = EvaluateLTCRect(c_Normal, c_ViewDir, v_FragPos, mat3(1.0), u_AreaLight[i].Points, u_AreaLight[i].TwoSided);
    specular = EvaluateLTCRect(c_Normal, c_ViewDir, v_FragPos, Minv, u_AreaLight[i].Points, u_AreaLight[i].TwoSided);
    // GGX BRDF shadowing and Fresnel
    // t2.x: shadowedF90 (F90 normally it should be 1.0)
    // t2.y: Smith function for Geometric Attenuation Term, it is dot(V or L, H).
    specular *= vec3(c_Metallic) * t2.x + (1.0 - vec3(c_Metallic)) * t2.y;
    lighting = u_AreaLight[i].Color * u_AreaLight[i].Intensity * (specular + c_Albedo * diffuse);

    return lighting;
}

vec3 CalcAreaLightDisk(int i)
{
    vec3 lighting = vec3(0.0);
    float dotNV = clamp(dot(c_Normal, c_ViewDir), 0.0, 1.0);
    // use roughness and sqrt(1-cos_theta) to sample M_texture
    vec2 uv = vec2(c_Roughness, sqrt(1.0 - dotNV));
    uv = uv * LUT_SCALE + LUT_BIAS;
    // get 4 parameters for inverse_M
    vec4 t1 = texture(u_LTC1, uv);
    // Get 2 parameters for Fresnel calculation
    vec4 t2 = texture(u_LTC2, uv);
    mat3 Minv = mat3(
        vec3(t1.x, 0.0, t1.y),
        vec3(0.0,  1.0,  0.0),
        vec3(t1.z, 0.0, t1.w)
    );
    // Evaluate LTC shading
    vec3 diffuse, specular;
    diffuse = EvaluateLTCDisk(c_Normal, c_ViewDir, v_FragPos, mat3(1.0), u_AreaLight[i].Points, u_AreaLight[i].TwoSided);
    specular = EvaluateLTCDisk(c_Normal, c_ViewDir, v_FragPos, Minv, u_AreaLight[i].Points, u_AreaLight[i].TwoSided);
    specular *= vec3(c_Metallic) * t2.x + (1.0 - vec3(c_Metallic)) * t2.y;
    lighting = u_AreaLight[i].Color * u_AreaLight[i].Intensity * (specular + c_Albedo * diffuse);
    
    lighting.x = max(0.0, lighting.x);
    lighting.y = max(0.0, lighting.y);
    lighting.z = max(0.0, lighting.z);

    return lighting; 
}

vec3 CalcAreaLightCylinder(int i)
{
    vec3 lighting = vec3(0.0);
    float dotNV = clamp(dot(c_Normal, c_ViewDir), 0.0, 1.0);
    // use roughness and sqrt(1-cos_theta) to sample M_texture
    vec2 uv = vec2(c_Roughness, sqrt(1.0 - dotNV));
    uv = uv * LUT_SCALE + LUT_BIAS;
    // get 4 parameters for inverse_M
    vec4 t1 = texture(u_LTC1, uv);
    // Get 2 parameters for Fresnel calculation
    vec4 t2 = texture(u_LTC2, uv);
    mat3 Minv = mat3(
        vec3(t1.x, 0.0, t1.y),
        vec3(0.0,  1.0,  0.0),
        vec3(t1.z, 0.0, t1.w)
    );

    // Evaluate LTC shading
    vec3 diffuse, specular;
    diffuse = EvaluateLTCCylinder(c_Normal, c_ViewDir, v_FragPos, mat3(1.0), u_AreaLight[i].Points, u_AreaLight[i].Scale.z, u_AreaLight[i].TwoSided);
    specular = EvaluateLTCCylinder(c_Normal, c_ViewDir, v_FragPos, Minv, u_AreaLight[i].Points, u_AreaLight[i].Scale.z, u_AreaLight[i].TwoSided);
    specular *= vec3(c_Metallic) * t2.x + (1.0 - vec3(c_Metallic)) * t2.y;
    lighting = u_AreaLight[i].Color * u_AreaLight[i].Intensity * (specular + c_Albedo * diffuse);

    return lighting;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// P is fragPos in world space (LTC distribution)
vec3 EvaluateLTCRect(vec3 N, vec3 V, vec3 P, mat3 Minv, vec3 vertex[4], bool twoSided)
{
    // construct orthonormal basis around N
    vec3 T1, T2;
    T1 = normalize(V - N * dot(V, N));
    T2 = cross(N, T1);
    // rotate area light in (T1, T2, N) basis
    Minv = Minv * transpose(mat3(T1, T2, N));
    // polygon (allocate 4 vertices for clipping)
    vec3 L[4];
    // transform polygon from LTC back to origin Do (cosine weighted)
    L[0] = Minv * (vertex[0] - P);
    L[1] = Minv * (vertex[1] - P);
    L[2] = Minv * (vertex[2] - P);
    L[3] = Minv * (vertex[3] - P);
    // use tabulated horizon-clipped sphere
    // check if the shading point is behind the light
    vec3 dir = vertex[0] - P; // LTC space
    vec3 lightNormal = cross(vertex[1] - vertex[0], vertex[3] - vertex[0]);
    bool behind = (dot(dir, lightNormal) < 0.0);
    // cos weighted space
    L[0] = normalize(L[0]);
    L[1] = normalize(L[1]);
    L[2] = normalize(L[2]);
    L[3] = normalize(L[3]);
    // integrate
    vec3 vsum = vec3(0.0);
    vsum += IntegrateEdgeVec(L[0], L[1]);
    vsum += IntegrateEdgeVec(L[1], L[2]);
    vsum += IntegrateEdgeVec(L[2], L[3]);
    vsum += IntegrateEdgeVec(L[3], L[0]);
    // form factor of the polygon in direction vsum
    float len = length(vsum);
    float z = vsum.z / len;
    if (behind)
    {
        z = -z;
    }
    vec2 uv = vec2(z * 0.5 + 0.5, len); // range [0, 1]
    uv = uv * LUT_SCALE + LUT_BIAS;
    // Fetch the form factor for horizon clipping
    float scale = texture(u_LTC2, uv).w;
    float sum = len * scale;
    if (!behind && !twoSided)
    {
        sum = 0.0;
    }
    // Outgoing radiance (solid angle) for the entire polygon
    vec3 Lo_i = vec3(sum);
    return Lo_i;
}

vec3 EvaluateLTCDisk(vec3 N, vec3 V, vec3 P, mat3 Minv, vec3 vertex[4], bool twoSided)
{
    // construct orthonormal basis around N
    vec3 T1, T2;
    T1 = normalize(V - N * dot(V, N));
    T2 = cross(N, T1);
    // rotate area light in (T1, T2, N) basis
    mat3 R = transpose(mat3(T1, T2, N));

    // polygon (allocate 5 vertices for clipping)
    vec3 L_[3];
    L_[0] = R * (vertex[0] - P);
    L_[1] = R * (vertex[1] - P);
    L_[2] = R * (vertex[2] - P);

    // init ellipse
    vec3 C  = 0.5 * (L_[0] + L_[2]);
    vec3 V1 = 0.5 * (L_[1] - L_[2]);
    vec3 V2 = 0.5 * (L_[1] - L_[0]);

    C  = Minv * C;
    V1 = Minv * V1;
    V2 = Minv * V2;

    if(!twoSided && dot(cross(V1, V2), C) >= 0.0)
    {
        return vec3(0.0);
    }

    // compute eigenvectors of ellipse
    float a, b;
    float d11 = dot(V1, V1);
    float d22 = dot(V2, V2);
    float d12 = dot(V1, V2);

    if (abs(d12) / sqrt(d11 * d22) > 0.0001)
    //if (true)
    {
        float tr = d11 + d22;
        float det = -d12 * d12 + d11 * d22;

        // use sqrt matrix to solve for eigenvalues
        det = sqrt(det);
        float u = 0.5 * sqrt(tr - 2.0 * det);
        float v = 0.5 * sqrt(tr + 2.0 * det);
        float e_max = (u + v) * (u + v);
        float e_min = (u - v) * (u - v);

        vec3 V1_, V2_;

        if (d11 > d22)
        {
            V1_ = d12 * V1 + (e_max - d11) * V2;
            V2_ = d12 * V1 + (e_min - d11) * V2;
        }
        else
        {
            V1_ = d12 * V2 + (e_max - d22) * V1;
            V2_ = d12 * V2 + (e_min - d22) * V1;
        }

        a = 1.0 / e_max;
        b = 1.0 / e_min;
        V1 = normalize(V1_);
        V2 = normalize(V2_);
    }
    else
    {
        a = 1.0 / dot(V1, V1);
        b = 1.0 / dot(V2, V2);
        V1 *= sqrt(a);
        V2 *= sqrt(b);
    }

    vec3 V3 = cross(V1, V2);
    if (dot(C, V3) < 0.0)
    {
        V3 *= -1.0;
    }

    float L  = dot(V3, C);
    float x0 = dot(V1, C) / L;
    float y0 = dot(V2, C) / L;

    float E1 = inversesqrt(a);
    float E2 = inversesqrt(b);

    a *= L * L;
    b *= L * L;

    float c0 = a * b;
    float c1 = a * b * (1.0 + x0 * x0 + y0 * y0) - a - b;
    float c2 = 1.0 - a * (1.0 + x0 * x0) - b * (1.0 + y0 * y0);
    float c3 = 1.0;

    vec3 roots = SolveCubic(vec4(c0, c1, c2, c3));
    float e1 = roots.x;
    float e2 = roots.y;
    float e3 = roots.z;

    vec3 avgDir = vec3(a * x0 / (a - e2), b * y0 / (b - e2), 1.0);
    mat3 rotate = mat3(V1, V2, V3);

    avgDir = rotate * avgDir;
    avgDir = normalize(avgDir);

    float L1 = sqrt(-e2 / e3);
    float L2 = sqrt(-e2 / e1);

    float formFactor = L1 * L2 * inversesqrt((1.0 + L1 * L1) * (1.0 + L2 * L2));

    // use tabulated horizon-clipped sphere
    vec2 uv = vec2(avgDir.z * 0.5 + 0.5, formFactor);
    uv = uv * LUT_SCALE + LUT_BIAS;
    float scale = texture(u_LTC2, uv).w;

    float spec = formFactor * scale;
    return vec3(spec);
}

vec3 EvaluateLTCCylinder(vec3 N, vec3 V, vec3 P, mat3 Minv, vec3 vertex[4], float radius, bool endCaps)
{
    // construct orthonormal basis around N
    vec3 T1, T2;
    T1 = normalize(V - N * dot(V, N));
    T2 = cross(N, T1);
    mat3 B = transpose(mat3(T1, T2, N));
    vec3 p1 = B * (vertex[0] - P);
    vec3 p2 = B * (vertex[1] - P);
    // transform to diffuse configuration
    vec3 p1o = Minv * p1;
    vec3 p2o = Minv * p2;

    float I_diffuse = 0.0;
    // tangent
    vec3 wt = normalize(p2o - p1o);
    // clamping
    if (p1o.z <= 0.0 && p2o.z <= 0.0)
    {
       I_diffuse = 0.0; 
    }
    else
    {
        if (p1o.z < 0.0)
        {
            p1o = (+p1o * p2o.z - p2o * p1o.z) / (+p2o.z - p1o.z);
        }

        if (p2o.z < 0.0)
        {
            p2o = (-p1o * p2o.z + p2o * p1o.z) / (-p2o.z + p1o.z);
        }
        // parameterization Eq.(1.12, 1.13)
        float l1 = dot(p1o, wt);
        float l2 = dot(p2o, wt);
        // shading point orthonormal projection on the line Eq.(1.14)
        vec3 po = p1o - l1*wt;
        // distance to line Eq.(1.15)
        float d = length(po);
        // integral Eq.(1.21)
        float I = (Fpo(d, l2) - Fpo(d, l1)) * po.z + (Fwt(d, l2) - Fwt(d, l1)) * wt.z;
        I_diffuse = I / PI;
    }
    // width factor
    vec3 ortho = normalize(cross(p1, p2));
    float w =  1.0 / length(inverse(transpose(Minv)) * ortho);
    float Iline = radius * w * I_diffuse;

    float Idisks = 0.0;
    if (endCaps)
    {
        float A = PI * radius * radius;
        vec3 wt  = normalize(p2 - p1);
        vec3 wp1 = normalize(p1);
        vec3 wp2 = normalize(p2);
        Idisks = A * (
        D(wp1, Minv) * max(0.0, dot(+wt, wp1)) / dot(p1, p1) +
        D(wp2, Minv) * max(0.0, dot(-wt, wp2)) / dot(p2, p2));
    }
    
    // there are some bugs when roughness is quite small
    return vec3(min(1.0, Iline + Idisks));
}

// Vector form without project to the plane (dot with the normal)
// Use for proxy sphere clipping
vec3 IntegrateEdgeVec(vec3 v1, vec3 v2)
{
    // Using built-in acos() function will result flaws
    // Using fitting result for calculating acos()
    float x = dot(v1, v2);
    float y = abs(x);

    float a = 0.8543985 + (0.4965155 + 0.0145206 * y) * y;
    float b = 3.4175940 + (4.1616724 + y) * y;
    float v = a / b;

    float theta_sintheta = x > 0.0 ? v : 0.5 * inversesqrt(max(1.0 - x * x, 1e-7)) - v;

    return cross(v1, v2) * theta_sintheta;
}

float IntegrateEdge(vec3 v1, vec3 v2)
{
    return IntegrateEdgeVec(v1, v2).z;
}

// An extended version of the implementation from
// "How to solve a cubic equation, revisited"
// http://momentsingraphics.de/?p=105
vec3 SolveCubic(vec4 Coefficient)
{
    // Normalize the polynomial
    Coefficient.xyz /= Coefficient.w;
    // Divide middle coefficients by three
    Coefficient.yz /= 3.0;

    float A = Coefficient.w;
    float B = Coefficient.z;
    float C = Coefficient.y;
    float D = Coefficient.x;

    // Compute the Hessian and the discriminant
    vec3 Delta = vec3(
        -Coefficient.z * Coefficient.z + Coefficient.y,
        -Coefficient.y * Coefficient.z + Coefficient.x,
        dot(vec2(Coefficient.z, -Coefficient.y), Coefficient.xy)
    );

    float Discriminant = dot(vec2(4.0 * Delta.x, -Delta.y), Delta.zy);

    vec3 RootsA, RootsD;

    vec2 xlc, xsc;

    // Algorithm A
    {
        float A_a = 1.0;
        float C_a = Delta.x;
        float D_a = -2.0 * B * Delta.x + Delta.y;

        // Take the cubic root of a normalized complex number
        float Theta = atan(sqrt(Discriminant), -D_a) / 3.0;

        float x_1a = 2.0 * sqrt(-C_a) * cos(Theta);
        float x_3a = 2.0 * sqrt(-C_a) * cos(Theta + (2.0 / 3.0) * PI);

        float xl;
        if ((x_1a + x_3a) > 2.0 * B)
        {
            xl = x_1a;
        }
        else
        {
            xl = x_3a;   
        }

        xlc = vec2(xl - B, A);
    }

    // Algorithm D
    {
        float A_d = D;
        float C_d = Delta.z;
        float D_d = -D * Delta.y + 2.0 * C * Delta.z;

        // Take the cubic root of a normalized complex number
        float Theta = atan(D * sqrt(Discriminant), -D_d) / 3.0;

        float x_1d = 2.0 * sqrt(-C_d) * cos(Theta);
        float x_3d = 2.0 * sqrt(-C_d) * cos(Theta + (2.0 / 3.0) * PI);

        float xs;
        if (x_1d + x_3d < 2.0 * C)
        {
            xs = x_1d;
        }
        else
        {
            xs = x_3d;
        }

        xsc = vec2(-D, xs + C);
    }

    float E =  xlc.y * xsc.y;
    float F = -xlc.x * xsc.y - xlc.y * xsc.x;
    float G =  xlc.x * xsc.x;

    vec2 xmc = vec2(C * F - B * G, -B * F + C * E);

    vec3 Root = vec3(xsc.x / xsc.y, xmc.x / xmc.y, xlc.x / xlc.y);

    if (Root.x < Root.y && Root.x < Root.z)
    {
        Root.xyz = Root.yxz;
    }
    else if (Root.z < Root.x && Root.z < Root.y)
    {
        Root.xyz = Root.xzy;
    }

    return Root;
}

float Fpo(float d, float l)
{
    return l/(d*(d*d + l*l)) + atan(l/d)/(d*d);
}

float Fwt(float d, float l)
{
    return l*l/(d*(d*d + l*l));
}

float D(vec3 w, mat3 Minv)
{
    // Using Minv to get back to origin distribution
    vec3 wo = Minv * w;
    float lo = length(wo);
    // BRDF * cos
    float res = 1.0/PI * max(0.0, wo.z/lo) * abs(determinant(Minv)) / (lo*lo*lo);
    return res;
}