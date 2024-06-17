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
uniform bool u_Horizontal;

const float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{             
     vec2 tex_offset = 1.0 / textureSize(u_ScreenTex, 0); // gets size of single texel
     vec3 result = texture(u_ScreenTex, v_TexCoord).rgb * weight[0];
     if(u_Horizontal)
     {
         for(int i = 1; i < 5; ++i)
         {
            result += texture(u_ScreenTex, v_TexCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(u_ScreenTex, v_TexCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
         }
     }
     else
     {
         for(int i = 1; i < 5; ++i)
         {
             result += texture(u_ScreenTex, v_TexCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
             result += texture(u_ScreenTex, v_TexCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
         }
     }
     v_FragColor = vec4(result, 1.0);
}