#SHADER VERTEX
#version 460 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;
out vec4 v_Offset[3];

uniform vec4 u_Metrics;

vec4 mad(vec4 a, vec4 b, vec4 c)
{
    return a * b + c;
}

void main()
{
    v_TexCoord = vec2((a_Position + 1.0) / 2.0);
    v_Offset[0] = mad(u_Metrics.xyxy, vec4(-1.0, 0.0, 0.0, -1.0), v_TexCoord.xyxy);
    v_Offset[1] = mad(u_Metrics.xyxy, vec4( 1.0, 0.0, 0.0,  1.0), v_TexCoord.xyxy);
    v_Offset[2] = mad(u_Metrics.xyxy, vec4(-2.0, 0.0, 0.0, -2.0), v_TexCoord.xyxy);
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);   
} 



#SHADER FRAGMENT
#version 460 core

precision highp float;

layout (location = 0) out vec4 v_FragColor;

in vec2 v_TexCoord;
in vec4 v_Offset[3];

precision highp float;

#define THRESHOLD 0.1
#define LOCAL_CONTRAST_ADAPTATION_FACTOR 2.0

uniform sampler2D u_ColorTex;

void main() 
{
    vec2 threshold = vec2(THRESHOLD);

    // Calculate lumas:
    vec3 weights = vec3(0.2126, 0.7152, 0.0722);
    float L = dot(texture2D(u_ColorTex, v_TexCoord).rgb, weights);

    float Lleft = dot(texture2D(u_ColorTex, v_Offset[0].xy).rgb, weights);
    float Ltop  = dot(texture2D(u_ColorTex, v_Offset[0].zw).rgb, weights);

    // We do the usual threshold:
    vec4 delta;
    delta.xy = abs(L - vec2(Lleft, Ltop));
    vec2 edges = step(threshold, delta.xy);

    // Then discard if there is no edge:
    if (dot(edges, vec2(1.0, 1.0)) == 0.0)
    {
        v_FragColor = vec4(0.0);
    }
    else
    {
        // Calculate right and bottom deltas:
        float Lright = dot(texture2D(u_ColorTex, v_Offset[1].xy).rgb, weights);
        float Lbottom  = dot(texture2D(u_ColorTex, v_Offset[1].zw).rgb, weights);
        delta.zw = abs(L - vec2(Lright, Lbottom));
        // Calculate the maximum delta in the direct neighborhood:
        vec2 maxDelta = max(delta.xy, delta.zw);
        // Calculate left-left and top-top deltas:
        float Lleftleft = dot(texture2D(u_ColorTex, v_Offset[2].xy).rgb, weights);
        float Ltoptop = dot(texture2D(u_ColorTex, v_Offset[2].zw).rgb, weights);
        delta.zw = abs(vec2(Lleft, Ltop) - vec2(Lleftleft, Ltoptop));
        // Calculate the final maximum delta:
        maxDelta = max(maxDelta.xy, delta.zw);
        float finalDelta = max(maxDelta.x, maxDelta.y);
        // Local contrast adaptation:
        edges.xy *= step(finalDelta, LOCAL_CONTRAST_ADAPTATION_FACTOR * delta.xy);

        v_FragColor = vec4(edges, 0.0, 1.0);
    }
}