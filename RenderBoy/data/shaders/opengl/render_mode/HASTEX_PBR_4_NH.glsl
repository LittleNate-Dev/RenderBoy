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
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_BiTangent;
layout (location = 5) in vec4 a_TexIndex;
layout (location = 6) in vec3 a_ColorIndex;
layout (location = 7) in vec4 a_AttributeIndex;
layout (location = 8) in vec2 a_NHIndex;
layout (location = 9) in mat4 a_ModelMat;

out vec3 v_FragPos;
out vec3 v_Normal;
out mat3 v_TBN;
out vec2 v_TexCoord;
out vec4 v_TexIndex;
out vec3 v_ColorIndex;
out vec4 v_AttributeIndex;
out vec2 v_NHIndex;
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
    v_NHIndex = a_NHIndex;
    if (a_Normal == vec3(0.0))
    {
        v_Normal = vec3(0.0);
    }
    else
    {
        mat3 normalMat = transpose(inverse(mat3(a_ModelMat)));
        v_Normal = normalMat * a_Normal;
        vec3 T = normalize(normalMat * a_Tangent);
        vec3 B = normalize(normalMat * a_BiTangent);
        vec3 N = normalize(v_Normal);
        v_TBN = mat3(T, B, N);
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
#define LUT_SIZE 64.0
#define LUT_SCALE (LUT_SIZE - 1.0)/LUT_SIZE;
#define LUT_BIAS 0.5/LUT_SIZE;

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
    vec3 Position;
    vec3 RecVertex[4];
    float Intensity;
    vec3 Color;
    bool TwoSided;
    bool LightSwitch;
};

layout(location = 0) out vec4 v_Accum;
layout(location = 1) out float v_Reveal;

in vec3 v_FragPos;
in vec3 v_Normal;
in mat3 v_TBN;
in vec2 v_TexCoord;
in vec4 v_TexIndex;
in vec3 v_ColorIndex;
in vec4 v_AttributeIndex;
in vec2 v_NHIndex;
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
uniform sampler2D u_NormalTex[];
uniform sampler2D u_HeightTex[];

uniform bool u_OITPass;

vec2 c_TexCoord = vec2(0.0);
vec3 c_ViewDir = vec3(0.0);
vec3 c_Normal = vec3(0.0);
float c_SSAO = 1.0;
vec3 c_ColorIndex = vec3(-1.0);
vec4 c_AttributeIndex = vec4(-1.0);
int c_AlbedoTexIndex = -1;
int c_MetallicTexIndex = -1;
int c_RoughnessTexIndex = -1;
int c_AoTexIndex = -1;
int c_NormalTexIndex = -1;
int c_HeightTexIndex = -1;
vec3 c_Albedo = vec3(0.0);
float c_Metallic = 0.0;
float c_Roughness = 1.0;
float c_AO = 1.0;
vec3 c_F0 = vec3(0.04);
float c_Alpha = 1.0;

vec3 CalcPointLight(int i);
vec3 CalcSpotLight(int i);
vec3 CalcDirLight(int i);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);
vec3 EvaluateLTC(vec3 N, vec3 V, vec3 P, mat3 Minv, vec3 vertex[4], bool twoSided);
vec3 IntegrateEdgeVec(vec3 v1, vec3 v2);
float IntegrateEdge(vec3 v1, vec3 v2);

void main()
{
    c_TexCoord = v_TexCoord;
    c_AlbedoTexIndex = int(v_TexIndex.x + 0.1);
    c_MetallicTexIndex = int(v_TexIndex.y + 0.1);
    c_RoughnessTexIndex = int(v_TexIndex.z + 0.1);
    c_AoTexIndex = int(v_TexIndex.w + 0.1);
    c_ColorIndex = v_ColorIndex + vec3(0.1);
    c_AttributeIndex = v_AttributeIndex + vec4(0.1);
    c_NormalTexIndex = int(v_NHIndex.x + 0.1);
    c_HeightTexIndex = int(v_NHIndex.y + 0.1);
    // Varibles used for lighting
    c_ViewDir = normalize(u_ViewPos - v_FragPos);
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
    // Height mapping
    if (v_NHIndex.y >= 0 && v_NHIndex.y >= 0)
    { 
        vec3 viewDirTBN = normalize(transpose(v_TBN) * c_ViewDir);
        const float numLayers = 10;
        float layerDepth = 1.0 / numLayers;
        float currentLayerDepth = 0.0;
        vec2 P = viewDirTBN.xy * 0.2; 
        vec2 deltaTexCoords = P / numLayers;
        vec2  currentTexCoords = c_TexCoord;
        float currentDepthMapValue = texture(u_HeightTex[c_HeightTexIndex], c_TexCoord).r;
  
        while(currentLayerDepth < currentDepthMapValue)
        {
            currentTexCoords -= deltaTexCoords;
            currentDepthMapValue = texture(u_HeightTex[c_HeightTexIndex], currentTexCoords).r;  
            currentLayerDepth += layerDepth;  
        }
        vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
        float afterDepth  = currentDepthMapValue - currentLayerDepth;
        float beforeDepth = texture(u_HeightTex[c_HeightTexIndex], prevTexCoords).r - currentLayerDepth + layerDepth;
        float weight = afterDepth / (afterDepth - beforeDepth);
        c_TexCoord = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
        if(c_TexCoord.x > 1.0 || c_TexCoord.y > 1.0 || c_TexCoord.x < 0.0 || c_TexCoord.y < 0.0)
        {
            discard;
        }
    }
    
    // Normal mapping
    if (v_NHIndex.x < 0)
    {
        c_Normal = normalize(v_Normal);
    } 
    else
    {
        c_Normal = texture(u_NormalTex[c_NormalTexIndex], c_TexCoord).rgb;
        c_Normal = normalize(c_Normal * 2.0 - 1.0);
        c_Normal = normalize(v_TBN * c_Normal);
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
        c_Albedo = texture(u_AlbedoTex[c_AlbedoTexIndex], c_TexCoord).rgb;
        c_Metallic = texture(u_MetallicTex[c_MetallicTexIndex], c_TexCoord).b;
        c_Roughness = texture(u_RoughnessTex[c_RoughnessTexIndex], v_TexCoord).r;
    }
    if (v_TexIndex.w >= 0)
    {
        c_AO = texture(u_AoTex[c_AoTexIndex], c_TexCoord).r;
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
        float bias = max(u_SpotLight[i].Bias.x * (1.0 - dot(normalize(v_Normal), normalize(u_SpotLight[i].Position - v_FragPos))), u_SpotLight[i].Bias.y);
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
    float dotNV = clamp(dot(c_Normal, c_ViewDir), 0.0f, 1.0f);
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
    vec3 diffuse = EvaluateLTC(c_Normal, c_ViewDir, v_FragPos, mat3(1.0), u_AreaLight[i].RecVertex, u_AreaLight[i].TwoSided);
    vec3 specular = EvaluateLTC(c_Normal, c_ViewDir, v_FragPos, Minv, u_AreaLight[i].RecVertex, u_AreaLight[i].TwoSided);
    // GGX BRDF shadowing and Fresnel
    // t2.x: shadowedF90 (F90 normally it should be 1.0)
    // t2.y: Smith function for Geometric Attenuation Term, it is dot(V or L, H).
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
vec3 EvaluateLTC(vec3 N, vec3 V, vec3 P, mat3 Minv, vec3 vertex[4], bool twoSided)
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