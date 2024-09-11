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
uniform sampler2D u_DOF;

void main()
{
    vec4 source = texture(u_Source, v_TexCoord);
	float coc = source.a;
	vec4 dof = texture(u_DOF, v_TexCoord);

	float dofStrength = smoothstep(0.1, 1.0, abs(coc));
	float weight = dofStrength + dof.a - dofStrength * dof.a;
	vec3 color = source.rgb + (dof.rgb - source.rgb) * weight;

	v_FragColor = vec4(color, 1.0);
}