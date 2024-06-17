#SHADER VERTEX
#version 460 core

layout(location = 0) in vec4 a_Position;

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ModelMat;

void main()
{
    gl_Position = u_ProjMat * u_ViewMat * u_ModelMat * a_Position;
}



#SHADER GEOMETRY
#version 460 core

layout(triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 v_EdgeDistance;

uniform mat4 u_ViewPortMat;

void main()
{
    vec4 p;

    p = gl_in[0].gl_Position;
    vec2 p0 = vec2(u_ViewPortMat * (p / p.w));

    p = gl_in[1].gl_Position;
    vec2 p1 = vec2(u_ViewPortMat * (p / p.w));

    p = gl_in[2].gl_Position;
    vec2 p2 = vec2(u_ViewPortMat * (p / p.w));

    float a = length(p1 - p2);
    float b = length(p2 - p0);
    float c = length(p1 - p0);

    float alpha = acos((b * b + c * c - a * a) / (2.0 * b *c));
    float beta = acos((a * a + c * c - b * b) / (2.0 * a * c));

    float ha = abs(c * sin(beta));
    float hb = abs(c * sin(alpha));
    float hc = abs(b * sin(alpha));

    v_EdgeDistance = vec3(ha, 0.0, 0.0);
    gl_Position = gl_in[0].gl_Position;
    gl_ClipDistance = gl_in[0].gl_ClipDistance;
    EmitVertex();

    v_EdgeDistance = vec3(0.0, hb, 0.0);
    gl_Position = gl_in[1].gl_Position;
    gl_ClipDistance = gl_in[1].gl_ClipDistance;
    EmitVertex();

    v_EdgeDistance = vec3(0.0, 0.0, hc);
    gl_Position = gl_in[2].gl_Position;
    gl_ClipDistance = gl_in[2].gl_ClipDistance;
    EmitVertex();

    EndPrimitive();
}



#SHADER FRAGMENT
#version 460 core

layout(location = 0) out vec4 v_FragColor;

in vec3 v_EdgeDistance;

uniform vec2 u_RenderRes;
uniform vec3 u_Color;
uniform float u_Intensity;

const vec4 lineColor = vec4(0.0, 0.0, 0.0, 1.0);

void main()
{
    float lineWidth = (u_RenderRes.x / 1920.0) * 0.5 + (u_RenderRes.y / 1080.0) * 0.5;
    float d = min(v_EdgeDistance.x, min(v_EdgeDistance.y, v_EdgeDistance.z));
    float mixVal = 0.0;
    if (d < lineWidth - 1)
    {
        mixVal = 1.0;
    }
    else if (d > lineWidth + 1)
    {
        mixVal = 0.0;
    }
    else
    {
        float x = d - (lineWidth - 1);
        mixVal = exp2(-2.0 * x * x);
    }
    v_FragColor = mix(vec4(u_Color * u_Intensity, 1.0), lineColor, mixVal);
    //v_FragColor = vec4(u_Color.r, u_Color.g, u_Color.b, 1.0);
}