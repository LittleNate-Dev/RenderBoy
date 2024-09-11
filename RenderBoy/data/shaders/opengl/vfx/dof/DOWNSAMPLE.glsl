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

uniform sampler2D u_Source;
uniform vec2 u_TexelSize;

float Weigh (vec3 c);

void main()
{
    vec4 o = u_TexelSize.xyxy * vec2(-0.5, 0.5).xxyy;

	vec4 source0 = texture(u_Source, v_TexCoord + o.xy);
	vec4 source1 = texture(u_Source, v_TexCoord + o.zy);
	vec4 source2 = texture(u_Source, v_TexCoord + o.xw);
	vec4 source3 = texture(u_Source, v_TexCoord + o.zw);

	vec3 s0 = source0.rgb;
	vec3 s1 = source1.rgb;
	vec3 s2 = source2.rgb;
	vec3 s3 = source3.rgb;

	float w0 = Weigh(s0);
	float w1 = Weigh(s1);
	float w2 = Weigh(s2);
	float w3 = Weigh(s3);

	vec3 color = s0 * w0 + s1 * w1 + s2 * w2 + s3 * w3;
	color /= max(w0 + w1 + w2 + s3, 0.00001);

	float coc0 = source0.a;
	float coc1 = source1.a;
	float coc2 = source2.a;
	float coc3 = source3.a;
	float cocMin = min(min(min(coc0, coc1), coc2), coc3);
	float cocMax = max(max(max(coc0, coc1), coc2), coc3);
	float coc = cocMax >= -cocMin ? cocMax : cocMin;

	v_FragColor = vec4(color, coc);
}

float Weigh (vec3 c) 
{
	return 1.0 / (1.0 + max(max(c.r, c.g), c.b));
}