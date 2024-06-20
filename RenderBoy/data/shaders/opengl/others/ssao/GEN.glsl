#SHADER VERTEX
#version 460 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0); 
} 



#SHADER FRAGMENT
#version 460 core
#extension GL_ARB_bindless_texture : require

out float v_FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_Position;
uniform sampler2D u_Normal;
uniform sampler2D u_NoiseTex;
uniform vec3 u_Samples[64];

// parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
uniform int u_KernelSize = 64;
uniform float u_Radius = 0.5;
uniform float u_Bias = 0.025;

// tile noise texture over screen based on screen dimensions divided by noise size
uniform vec2 u_NoiseScale; 

uniform mat4 u_ProjMat;

void main()
{
    // get input for SSAO algorithm
    vec3 fragPos = texture(u_Position, v_TexCoord).xyz;
    vec3 normal = normalize(texture(u_Normal, v_TexCoord).rgb);
    vec3 randomVec = normalize(texture(u_NoiseTex, v_TexCoord * u_NoiseScale).xyz);
    // create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < u_KernelSize; i++)
    {
        // get sample position
        vec3 samplePos = TBN * u_Samples[i]; // from tangent to view-space
        samplePos = fragPos + samplePos * u_Radius; 
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(samplePos, 1.0);
        offset = u_ProjMat * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sampleDepth = texture(u_Position, offset.xy).z; // get depth value of kernel sample
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, u_Radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + u_Bias ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / u_KernelSize);
    
    v_FragColor = occlusion;
}
