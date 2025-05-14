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

layout (location = 0) out vec4 v_FragColor;

in vec2 v_TexCoord;

// epsilon number
const float EPSILON = 0.00001f;

uniform sampler2D u_Accum;
uniform sampler2D u_Reveal;

bool IsApproximatelyEqual(float a, float b);
float Max3(vec3 v);

void main()
{
    // fragment revealage
    float revealage = texture(u_Reveal, v_TexCoord).r;

    // save the blending and color texture fetch cost if there is not a transparent fragment
    if (IsApproximatelyEqual(revealage, 1.0f))
    {
        discard;
    }

    // fragment color
    vec4 accumulation = texture(u_Accum, v_TexCoord);
    // suppress overflow
    if (isinf(Max3(abs(accumulation.rgb))))
    {
        accumulation.rgb = vec3(accumulation.a);
    }
    // prevent floating point precision bug
    vec3 average_color = accumulation.rgb / max(accumulation.a, EPSILON);
    // blend pixels
    v_FragColor = vec4(average_color, 1.0 - revealage);
}

// calculate floating point numbers equality accurately
bool IsApproximatelyEqual(float a, float b)
{
    return abs(a - b) <= (abs(a) < abs(b) ? abs(b) : abs(a)) * EPSILON;
}

// get the max value between three values
float Max3(vec3 v)
{
    return max(max(v.x, v.y), v.z);
}