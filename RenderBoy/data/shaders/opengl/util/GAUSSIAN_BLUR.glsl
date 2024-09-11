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

void main()
{
   vec4 o = u_TexelSize.xyxy * vec2(-0.5, 0.5).xxyy;
   vec3 s = 
		texture(u_Source, v_TexCoord + o.xy).rgb +
		texture(u_Source, v_TexCoord + o.zy).rgb +
		texture(u_Source, v_TexCoord + o.xw).rgb +
		texture(u_Source, v_TexCoord + o.zw).rgb;
	v_FragColor = vec4(s * 0.25, texture(u_Source, v_TexCoord).a);
}