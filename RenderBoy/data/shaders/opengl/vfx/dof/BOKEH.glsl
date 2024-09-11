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

#define PI 3.14159265359

uniform sampler2D u_Source;
uniform vec2 u_TexelSize;
uniform float u_Radius;

float Weigh (float coc, float radius);

void main()
{
    vec2 kernel[22] = vec2[](
		vec2(0, 0),
		vec2(0.53333336, 0),
		vec2(0.3325279, 0.4169768),
		vec2(-0.11867785, 0.5199616),
		vec2(-0.48051673, 0.2314047),
		vec2(-0.48051673, -0.23140468),
		vec2(-0.11867763, -0.51996166),
		vec2(0.33252785, -0.4169769),
		vec2(1, 0),
		vec2(0.90096885, 0.43388376),
		vec2(0.6234898, 0.7818315),
		vec2(0.22252098, 0.9749279),
		vec2(-0.22252095, 0.9749279),
		vec2(-0.62349, 0.7818314),
		vec2(-0.90096885, 0.43388382),
		vec2(-1, 0),
		vec2(-0.90096885, -0.43388376),
		vec2(-0.6234896, -0.7818316),
		vec2(-0.22252055, -0.974928),
		vec2(0.2225215, -0.9749278),
		vec2(0.6234897, -0.7818316),
		vec2(0.90096885, -0.43388376)
	);

	vec3 fgColor = vec3(0.0);
	vec3 bgColor = vec3(0.0);
	float fgWeight = 0.0, bgWeight = 0.0;
	float coc = texture(u_Source, v_TexCoord).a;
	vec3 red = vec3(0.5, 0.0, 0.0);
	vec3 blue = vec3(0.0, 0.0, 0.5);
	for (int k = 0; k < 22; k++) 
    {
		vec2 o = kernel[k] * u_Radius;
		float radius = length(o);
        o *= u_TexelSize;
		vec4 s = texture(u_Source, v_TexCoord + o);
		float bgw = Weigh(max(0, min(s.a, coc)), radius);
		bgColor += s.rgb * bgw;
		bgWeight += bgw;
		float fgw = Weigh(-s.a, radius);
		fgColor += s.rgb * fgw;
		fgWeight += fgw;
	}
	if (bgWeight == 0.0)
	{
		bgWeight += 1.0;
	}
	if (fgWeight == 0.0)
	{
		fgWeight += 1.0;
	}
	bgColor *= 1 / bgWeight;
	fgColor *= 1 / fgWeight;

	float bgfg = fgWeight * PI / 22.0;
	bgfg = texture(u_Source, v_TexCoord).a > 1.0 ? 0.0 : bgfg;
	bgfg = bgfg > 1.0 ? 1.0 : bgfg;
	vec3 color = bgColor + (fgColor - bgColor) * bgfg;
	v_FragColor = vec4(color, bgfg);
}

float Weigh (float coc, float radius) 
{
	float x = (coc - radius + 2.0) / 2.0;
	return max(0.0, min(1.0, x));
}