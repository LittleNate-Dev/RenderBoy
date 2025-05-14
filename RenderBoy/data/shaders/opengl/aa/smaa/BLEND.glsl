#SHADER VERTEX
#version 460 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;
out vec4 v_Offset;

uniform vec4 u_Metrics;

vec4 mad(vec4 a, vec4 b, vec4 c)
{
    return a * b + c;
}

void main()
{
    v_TexCoord = vec2((a_Position + 1.0) / 2.0);
    v_Offset = mad(u_Metrics.xyxy, vec4(1.0, 0.0, 0.0,  1.0), v_TexCoord.xyxy);
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);   
}



#SHADER FRAGMENT
#version 460 core

layout (location = 0) out vec4 v_FragColor;

precision highp float;

#define mad(a, b, c) (a * b + c)

in vec2 v_TexCoord;
in vec4 v_Offset;

uniform sampler2D u_ColorTex;
uniform sampler2D u_BlendTex;
uniform vec4 u_Metrics;

void Movc(bvec2 cond, inout vec2 variable, vec2 value);
void Movc(bvec4 cond, inout vec4 variable, vec4 value);

void main() 
{
    vec4 color;
    // Fetch the blending weights for current pixel:
    vec4 a;
    a.x = texture2D(u_BlendTex, v_Offset.xy).a; // Right
    a.y = texture2D(u_BlendTex, v_Offset.zw).g; // Top
    a.wz = texture2D(u_BlendTex, v_TexCoord).xz; // Bottom / Left

    // Is there any blending weight with a value greater than 0.0?
    if (dot(a, vec4(1.0, 1.0, 1.0, 1.0)) <= 1e-5) 
    {
        color = texture2D(u_ColorTex, v_TexCoord); // LinearSampler
    }
    else 
    {
        bool h = max(a.x, a.z) > max(a.y, a.w); // max(horizontal) > max(vertical)
        // Calculate the blending offsets:
        vec4 blendingOffset = vec4(0.0, a.y, 0.0, a.w);
        vec2 blendingWeight = a.yw;
        Movc(bvec4(h, h, h, h), blendingOffset, vec4(a.x, 0.0, a.z, 0.0));
        Movc(bvec2(h, h), blendingWeight, a.xz);
        blendingWeight /= dot(blendingWeight, vec2(1.0, 1.0));
        // Calculate the texture coordinates:
        vec4 blendingCoord = mad(blendingOffset, vec4(u_Metrics.xy, -u_Metrics.xy), v_TexCoord.xyxy);
        // We exploit bilinear filtering to mix current pixel with the chosen
        // neighbor:
        color = blendingWeight.x * texture2D(u_ColorTex, blendingCoord.xy); // LinearSampler
        color += blendingWeight.y * texture2D(u_ColorTex, blendingCoord.zw); // LinearSampler
    }    
    v_FragColor = color;
}

/**
 * Conditional move:
 */
void Movc(bvec2 cond, inout vec2 variable, vec2 value) 
{
    if (cond.x)
    {
        variable.x = value.x;
    }
    if (cond.y)
    {
        variable.y = value.y;
    }
}

void Movc(bvec4 cond, inout vec4 variable, vec4 value) 
{
    Movc(cond.xy, variable.xy, value.xy);
    Movc(cond.zw, variable.zw, value.zw);
}