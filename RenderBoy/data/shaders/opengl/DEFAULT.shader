#SHADER VERTEX
#version 450 core

#define POINT_LIGHT_COUNT
#define SPOT_LIGHT_COUNT
#define DIR_LIGHT_COUNT

struct SpotMat
{
    mat4 projMat;
    mat4 viewMat;
};

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_TexIndex;
layout (location = 6) in mat4 a_ModelMat;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_TexIndex;
out vec4 v_FragPosSpot[SPOT_LIGHT_COUNT];

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;
uniform SpotMat u_SpotMat[SPOT_LIGHT_COUNT];

void main()
{
    v_FragPos = vec3(a_ModelMat * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(a_ModelMat))) * a_Normal;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;

    for (int i = 0; i < SPOT_LIGHT_COUNT; i++)
    {
        v_FragPosSpot[i] = u_SpotMat[i].projMat * u_SpotMat[i].viewMat * vec4(v_FragPos, 1.0);
    }
    gl_Position = u_ProjMat * u_ViewMat * a_ModelMat * vec4(a_Position, 1.0);
}



#SHADER FRAGMENT
#version 450 core
#extension GL_ARB_bindless_texture : require

#define TEX_DIFFUSE
#define TEX_SPECULAR
#define TEX_NORMAL
#define POINT_LIGHT_COUNT
#define SPOT_LIGHT_COUNT
#define DIR_LIGHT_COUNT

struct PointLight
{
    vec3 position;  
    vec4 color;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float constant;
    float linear;
    float quadratic;
    float intensity;
    samplerCube shadowMap;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float angle;
    float dimAngle;
    vec4 color;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float constant;
    float linear;
    float quadratic;
    sampler2D shadowMap;
    float intensity;
};

struct DirLight
{
    vec3 direction;
    vec4 color;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct Material
{
    float shininess;
};

layout(location = 0) out vec4 v_FragColor;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_TexIndex;
in vec4 v_FragPosSpot[SPOT_LIGHT_COUNT];

uniform sampler2D u_TexDiffuse[TEX_DIFFUSE];
uniform sampler2D u_TexSpecular[TEX_SPECULAR];
uniform sampler2D u_TexNormal[TEX_NORMAL];
uniform vec3 u_ViewPos;
uniform float u_Gamma;
uniform Material u_Material;
uniform PointLight u_PointLight[POINT_LIGHT_COUNT];
uniform SpotLight u_SpotLight[SPOT_LIGHT_COUNT];
uniform DirLight u_DirLight[DIR_LIGHT_COUNT];

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, int count);
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec4 result = vec4(0.0);

    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);

    // Calculate all point lights
    for (int i = 0; i < POINT_LIGHT_COUNT; i++)
    {
        result += CalcPointLight(u_PointLight[i], norm, v_FragPos, viewDir);
    }
    for (int i = 0; i < SPOT_LIGHT_COUNT; i++)
    {
        result += CalcSpotLight(u_SpotLight[i], norm, viewDir, i);
    }
    for (int i = 0; i < DIR_LIGHT_COUNT; i++)
    {
        result += CalcDirLight(u_DirLight[i], norm, viewDir);
    }
    if (result.a < 0.1)
    {
        discard;
    }
    vec3 color = vec3(result);
    color = pow(color, vec3(1.0/u_Gamma));
    v_FragColor = vec4(color, 1.0);
}

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - v_FragPos);
    // diffuse shading
    float diff;
    if ((int(v_TexIndex.z + 0.5)) != -1)
    {
        //normal = texture(u_TexNormal[int(v_TexIndex.z + 0.5)], v_TexCoord).rgb;
        //normal = normalize(normal * 2.0 - 1.0);
    }
    if (normal == vec3(0.0))
    {
        diff = 1.0;
    }
    else
    {
        diff = max(dot(normal, lightDir), 0.0);
    }
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), u_Material.shininess);
    if (normal == vec3(0.0))
    {
        spec = 1.0;
    }
    else
    {
        spec = pow(max(dot(normal, halfwayDir), 0.0), u_Material.shininess);
    }
    // attenuation
    float distance = length(light.position - v_FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec4 ambient, diffuse, specular;
    ambient  = light.ambient * light.color  * texture(u_TexDiffuse[int(v_TexIndex.x + 0.5)], v_TexCoord);
    diffuse  = light.diffuse * light.color * diff * texture(u_TexDiffuse[int(v_TexIndex.x + 0.5)], v_TexCoord);
    if (int(v_TexIndex.x + 0.5) == -1)
    {
        specular = light.specular * spec * texture(u_TexSpecular[0], v_TexCoord);
    }
    else
    {
        specular = light.specular * spec * texture(u_TexSpecular[int(v_TexIndex.y + 0.5)], v_TexCoord);
    }
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
    float currentDepth = length(v_FragPos - light.position);
    float shadow = 0.0;
    float bias = 1.5;
    int samples = 20;
    float viewDistance = length(u_ViewPos - v_FragPos);
    float diskRadius = (1.0 + (viewDistance / light.intensity));
    if (currentDepth < light.intensity)
    {
        for(int i = 0; i < samples; ++i)
        {
            float closestDepth = texture(light.shadowMap, (v_FragPos - light.position) + gridSamplingDisk[i] * diskRadius).r;
            closestDepth *= light.intensity;   // undo mapping [0;1]
            if(currentDepth - bias > closestDepth)
            {
                shadow += 1.0;
            }   
        }
        shadow /= float(samples);
        lighting = ambient + (1.0 - shadow) * (diffuse + specular) * exp(-pow(currentDepth, 2) / (2 * (pow(light.intensity / 2, 2))));
    }
    else
    {
        lighting = ambient + (diffuse + specular) * exp(-pow(currentDepth, 2) / (2 * (pow(light.intensity / 2, 2))));
    }
    return lighting;
}

vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, int count)
{
    vec3 lightDir = normalize(light.position - v_FragPos);
    // diffuse shading
    float diff;
    if ((int(v_TexIndex.z + 0.5)) != -1)
    {
        //normal = texture(u_TexNormal[int(v_TexIndex.z + 0.5)], v_TexCoord).rgb;
        //normal = normalize(normal * 2.0 - 1.0);
    }
    if (normal == vec3(0.0))
    {
        diff = 1.0;
    }
    else
    {
        diff = max(dot(normal, lightDir), 0.0);
    }
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec;
    if (normal == vec3(0.0))
    {
        spec = 1.0;
    }
    else
    {
        spec = pow(max(dot(normal, halfwayDir), 0.0), u_Material.shininess);
    }
    // attenuation
    float distance = length(light.position - v_FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
    // dim out the light
    float theta = dot(-lightDir, normalize(light.direction));
    float epsilon = cos(light.angle/2) - cos(light.angle/2 + light.dimAngle);
    float intensity = clamp((theta - cos(light.angle/2 + light.dimAngle)) / epsilon, 0.0, 1.0);
    // combine results
    vec4 ambient, diffuse, specular;
    ambient  = light.ambient * light.color  * texture(u_TexDiffuse[int(v_TexIndex.x + 0.5)], v_TexCoord);
    diffuse  = light.diffuse * light.color * diff * texture(u_TexDiffuse[int(v_TexIndex.x + 0.5)], v_TexCoord);
    if (int(v_TexIndex.y + 0.5) == -1)
    {
        specular = light.specular * spec * texture(u_TexSpecular[0], v_TexCoord);
    }
    else
    {
        specular = light.specular * spec * texture(u_TexSpecular[int(v_TexIndex.y + 0.5)], v_TexCoord);
    }
    ambient *= attenuation;
    diffuse *= intensity * attenuation;
    specular *= intensity * attenuation;

    vec4 lighting = vec4(1.0);

    // Calculate Shadow
    vec3 ndc = v_FragPosSpot[count].xyz / v_FragPosSpot[count].w;
    // Inside light's frustum
    if (ndc.x < 1.0 && ndc.x > -1.0 && ndc.y < 1.0 && ndc.y > -1.0 && ndc.z < 1.0 )
    {
        float fragAngle = dot(-lightDir, light.direction);
        // Inside light's angle
        if (fragAngle > cos((light.angle / 2) + light.dimAngle))
        {
            vec2 shadowTex = ndc.xy;
            shadowTex = shadowTex * 0.5 + 0.5;
            float closestDepth = texture(light.shadowMap, shadowTex).r;
            float currentDepth = ndc.z * 0.5 + 0.5;
            float bias = max(0.000003 * (1.0 - dot(normal, lightDir)), 0.000001);
            // Inside shadow
            if (currentDepth - bias > closestDepth)
            {
                // PCF
                float shadow = 0.0;
                vec2 texelSize = 1.0 / textureSize(light.shadowMap, 0);
                for(int x = -1; x <= 1; ++x)
                {
                    for(int y = -1; y <= 1; ++y)
                    {
                        float pcfDepth = texture(light.shadowMap, shadowTex.xy + vec2(x, y) * texelSize).r; 
                        shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
                    }    
                }
                shadow /= 9.0;
                lighting = ambient + (1.0 - shadow) * (diffuse + specular);
            }
            else
            {
                lighting = ambient + (diffuse + specular) * exp(-pow(distance, 2) / (2 * (pow(light.intensity / 2, 2))));
            }
        }
        // Outside light's angle
        else
        {
            lighting = ambient + (diffuse + specular) * exp(-pow(distance, 2) / (2 * (pow(light.intensity / 2, 2))));
        }
    }
    // Outside light's frustum
    else
    {
        lighting = ambient + (diffuse + specular) * exp(-pow(distance, 2) / (2 * (pow(light.intensity / 2, 2))));
    }
    return lighting;
}

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff;
    if ((int(v_TexIndex.z + 0.5)) != -1)
    {
        //normal = texture(u_TexNormal[int(v_TexIndex.z + 0.5)], v_TexCoord).rgb;
        //normal = normalize(normal * 2.0 - 1.0);
    }
    if (normal == vec3(0.0))
    {
        diff = 1.0;
    }
    else
    {
        diff = max(dot(normal, lightDir), 0.0);
    }
    // specular shading
    float spec;
    if (normal == vec3(0.0))
    {
        spec = 1.0;
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    }
    // combine results
    vec4 ambient, diffuse, specular;
    ambient  = light.ambient * light.color  * texture(u_TexDiffuse[int(v_TexIndex.x + 0.5)], v_TexCoord);
    diffuse  = light.diffuse * light.color * diff * texture(u_TexDiffuse[int(v_TexIndex.x + 0.5)], v_TexCoord);
    if (int(v_TexIndex.y + 0.5) == -1)
    {
        specular = light.specular * spec * texture(u_TexSpecular[0], v_TexCoord);
    }
    else
    {
        specular = light.specular * spec * texture(u_TexSpecular[int(v_TexIndex.y + 0.5)], v_TexCoord);
    }
    
    return (ambient + diffuse + specular);
}  