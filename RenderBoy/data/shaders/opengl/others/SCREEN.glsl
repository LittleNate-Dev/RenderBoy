8#SHADER VERTEX
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

out vec4 v_FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_ScreenTex;
uniform float u_Gamma;
uniform float u_Exposure;
uniform int u_TonemapCurve;

float Tonemap(float x);
float Tonemap_Reinhard(float x);
float Tonemap_Reinhard2(float x);
float Tonemap_ACES(float x);
float Tonemap_Uchimura(float x);
float Tonemap_Lottes(float x);
vec3 RGBtoYXY(vec3 color);
vec3 YXYtoRGB(vec3 color);

void main()
{
    vec3 hdrColor = texture(u_ScreenTex, v_TexCoord).rgb;
    vec3 Yxy = RGBtoYXY(hdrColor);
    float lp = Yxy.x * u_Exposure;
    // Tone mapping
    Yxy.x = Tonemap(lp);
    vec3 result = YXYtoRGB(Yxy);
    result = pow(result, vec3(1.0 / u_Gamma));
    v_FragColor = vec4(result, 1.0);
}

float Tonemap(float x)
{
    float luminance = x;
    switch (u_TonemapCurve)
    {
    case 0:
        luminance = Tonemap_Reinhard(x);
        break;
    case 1:
        luminance = Tonemap_Reinhard2(x);
        break;
    case 2:
        luminance = Tonemap_ACES(x);
        break;
    case 3:
        luminance = Tonemap_Uchimura(x);
        break;
    case 4:
        luminance = Tonemap_Lottes(x);
        break;
    }
    return luminance;
}

float Tonemap_Reinhard(float x)
{
    return x / (1.0 + x);
}

float Tonemap_Reinhard2(float x)
{
    const float L_white = 4.0;
    return (x * (1.0 + x / (L_white * L_white))) / (1.0 + x);
}

float Tonemap_ACES(float x)
{
    // Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return (x * (a * x + b)) / (x * (c * x + d) + e);
}

float Tonemap_Uchimura(float x)
{
    // Uchimura 2017, "HDR theory and practice"
    // Math: https://www.desmos.com/calculator/gslcdxvipg
    // Source: https://www.slideshare.net/nikuque/hdr-theory-and-practicce-jp
//    const float P = 1.0;  // max display brightness
//    const float a = 1.0;  // contrast
//    const float m = 0.22; // linear section start
//    const float l = 0.4;  // linear section length
//    const float c = 1.33; // black
//    const float b = 0.0;  // pedestal
//    float l0 = ((P - m) * l) / a;
//    float L0 = m - m / a;
//    float L1 = m + (1.0 - m) / a;
//    float S0 = m + l0;
//    float S1 = m + a * l0;
//    float C2 = (a * P) / (P - S1);
//    float CP = -C2 / P;

    const float P = 1.0;
    const float a = 1.0;
    const float m = 0.22;
    const float c = 1.33;
    const float b = 0.0;
    const float l0 = 0.312;
    const float L0 = 0.0;
    const float L1 = 1.0;
    const float S0 = 0.532;
    const float S1 = 0.532;
    const float C2 = 2.13675;
    const float CP = -2.13675;

    float w0 = 1.0 - smoothstep(0.0, m, x);
    float w2 = step(m + l0, x);
    float w1 = 1.0 - w0 - w2;

    float T = m * pow(x / m, c) + b;
    float S = P - (P - S1) * exp(CP * (x - S0));
    float L = m + a * (x - m);

    return T * w0 + L * w1 + S * w2;
}

float Tonemap_Lottes(float x)
{
    // Lottes 2016, "Advanced Techniques and Optimization of HDR Color Pipelines"
    const float a = 1.6;
    const float d = 0.977;
    const float hdrMax = 8.0;
    const float midIn = 0.18;
    const float midOut = 0.267;

    // Can be precomputed
    //const float b =
        //(-pow(midIn, a) + pow(hdrMax, a) * midOut) /
        //((pow(hdrMax, a * d) - pow(midIn, a * d)) * midOut);
    //const float c =
        //(pow(hdrMax, a * d) * pow(midIn, a) - pow(hdrMax, a) * pow(midIn, a * d) * midOut) /
        //((pow(hdrMax, a * d) - pow(midIn, a * d)) * midOut);
    const float b = 1.07304;
    const float c = 0.16742;

    return pow(x, a) / (pow(x, a * d) * b + c);
}

vec3 RGBtoYXY(vec3 color)
{
    vec3 RGB = color / 255.0;

    if (RGB.r > 0.04045)
    {
        RGB.r = pow((RGB.r + 0.055) / 1.055, 2.4);
    }
    else
    {
        RGB.r = RGB.r / 12.92;
    }
    if (RGB.g > 0.04045)
    {
        RGB.g = pow((RGB.g + 0.055) / 1.055, 2.4);
    }
    else
    {
        RGB.g = RGB.g / 12.92;
    }
    if (RGB.b > 0.04045)
    {
        RGB.b = pow((RGB.b + 0.055) / 1.055, 2.4);
    }
    else
    {
        RGB.b = RGB.b / 12.92;
    }
    RGB *= 100.0;

    float X = RGB.r * 0.4124 + RGB.g * 0.3576 + RGB.b * 0.1805;
    float Y = RGB.r * 0.2126 + RGB.g * 0.7152 + RGB.b * 0.0722;
    float Z = RGB.r * 0.0193 + RGB.g * 0.1192 + RGB.b * 0.9505;
    float x = X / (X + Y + Z);
    float y = Y / (X + Y + Z);
    vec3 YXY = vec3(Y, x, y);
    return YXY;
}

vec3 YXYtoRGB(vec3 color)
{
    float X = color.y * (color.x / color.z);
    float Y = color.x;
    float Z = (1 - color.y - color.z) * (Y / color.z);

    vec3 XYZ = vec3(X, Y, Z);
    XYZ /= 100.0;

    float R = XYZ.x *  3.2406 + XYZ.y * -1.5372 + XYZ.z * -0.4986;
    float G = XYZ.x * -0.9689 + XYZ.y *  1.8758 + XYZ.z *  0.0415;
    float B = XYZ.x *  0.0557 + XYZ.y * -0.2040 + XYZ.z *  1.0570;

    if (R > 0.0031308)
    {
        R = 1.055 * pow(R, 1.0 / 2.4) - 0.055;
    }
    else
    {
        R = 12.92 * R;
    }
    if (G > 0.0031308)
    {
        G = 1.055 * pow(G, 1.0 / 2.4) - 0.055;
    }
    else
    {
        G = 12.92 * G;
    }
    if (B > 0.0031308)
    {
        B = 1.055 * pow(B, 1.0 / 2.4) - 0.055;
    }
    else
    {
        B = 12.92 * B;
    }
    vec3 RGB = vec3(R, G, B) * 255.0;
    return RGB;
}