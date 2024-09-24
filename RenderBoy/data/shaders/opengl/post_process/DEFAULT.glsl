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

out vec4 v_FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_ScreenTex;
uniform float u_Gamma;
uniform float u_Exposure;

vec3 RGBtoYXY(vec3 color);
vec3 YXYtoRGB(vec3 color);
float Tonemap_ACES(float x);

void main()
{
    vec3 hdrColor = texture(u_ScreenTex, v_TexCoord).rgb;
    vec3 Yxy = RGBtoYXY(hdrColor);
    float lp = Yxy.x * u_Exposure;

    // Tone mapping
    Yxy.x = Tonemap_ACES(lp);

    vec3 result = YXYtoRGB(Yxy);
    result = pow(result, vec3(1.0 / u_Gamma));
    v_FragColor = vec4(result, 1.0);
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