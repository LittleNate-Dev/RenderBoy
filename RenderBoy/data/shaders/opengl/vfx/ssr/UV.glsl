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

layout(location = 0) out vec4 v_FragColor;

in vec2 v_TexCoord;

uniform mat4 u_ProjMat;
uniform sampler2D u_PosTex;
uniform sampler2D u_NormalTex;
uniform sampler2D u_ColorTex;
uniform vec2 u_TexSize;
// Controls how far a fragment can reflect
uniform float u_MaxDistance;
// Controls how many fragments are skipped while traveling or marching the reflection ray during the first pass.
uniform float u_Resolution;
// Controls how many iterations occur during the second pass.
uniform int u_Step;
// Controls the cutoff between what counts as a possible reflection hit and what does not.
uniform float u_Thickness;
uniform int u_MaxLoop;

void main() 
{
    //v_FragColor = vec4(normalize(texture(u_NormalTex, v_TexCoord).xyz), 1.0);
    vec4 uv = vec4(0.0);

    vec4 positionFrom = texture(u_PosTex, v_TexCoord);

    if (positionFrom.w <= 0.0)
    { 
        v_FragColor = uv;
        return;
    }
    vec3 unitPositionFrom = normalize(positionFrom.xyz);
    vec3 normal = normalize(texture(u_NormalTex, v_TexCoord).xyz);
    vec3 pivot = normalize(reflect(unitPositionFrom, normal));
    vec4 positionTo = positionFrom;

    vec4 startView = vec4(positionFrom.xyz + (pivot * 0.0), 1.0);
    vec4 endView   = vec4(positionFrom.xyz + (pivot * u_MaxDistance), 1.0);

    vec4 startFrag = startView;
    startFrag = u_ProjMat * startFrag;
    startFrag.xyz /= startFrag.w;
    startFrag.xy = startFrag.xy * 0.5 + 0.5;
    startFrag.xy *= u_TexSize;

    vec4 endFrag = endView;
    endFrag = u_ProjMat * endFrag;
    endFrag.xyz /= endFrag.w;
    endFrag.xy = endFrag.xy * 0.5 + 0.5;
    endFrag.xy *= u_TexSize;

    vec2 frag = startFrag.xy;
    uv.xy = frag / u_TexSize;

    float deltaX = endFrag.x - startFrag.x;
    float deltaY = endFrag.y - startFrag.y;
    float useX = abs(deltaX) >= abs(deltaY) ? 1.0 : 0.0;
    float delta = mix(abs(deltaY), abs(deltaX), useX) * clamp(u_Resolution, 0.0, 1.0);
    vec2  increment = vec2(deltaX, deltaY) / max(delta, 0.001);

    float search0 = 0.0;
    float search1 = 0.0;

    int hit0 = 0;
    int hit1 = 0;

    float viewDistance = startView.y;
    float depth = u_Thickness;
    int loop = min(int(delta), u_MaxLoop);
    for (int i = 0; i < loop; i++)
    {
        frag += increment;
        uv.xy = frag / u_TexSize;

        positionTo = texture(u_PosTex, uv.xy);

        search1 = mix((frag.y - startFrag.y) / deltaY, (frag.x - startFrag.x) / deltaX, useX);
        search1 = clamp(search1, 0.0, 1.0);

        viewDistance = (startView.y * endView.y) / mix(endView.y, startView.y, search1);
        depth = viewDistance - positionTo.y;
        if (depth > 0.0 && depth < u_Thickness) 
        {
            hit0 = 1;
            break;
        }
        else
        {
            search0 = search1;
        }
    }

    search1 = search0 + ((search1 - search0) / 2.0);
    int steps = u_Step * hit0;
    loop = min(steps, u_MaxLoop);
    for (int i = 0; i < loop; i++)
    {
        frag = mix(startFrag.xy, endFrag.xy, search1);
        uv.xy = frag / u_TexSize;
        positionTo = texture(u_PosTex, uv.xy);

        viewDistance = (startView.y * endView.y) / mix(endView.y, startView.y, search1);
        depth = viewDistance - positionTo.y;
        if (depth > 0.0 && depth < u_Thickness)
        {
            hit1 = 1;
            search1 = search0 + ((search1 - search0) * 0.5);
        }
        else
        {
            float temp = search1;
            search1 = search1 + ((search1 - search0) * 0.5);
            search0 = temp;
         }
    }

    float visibility = hit1 * positionTo.w 
                    * (1- max(dot(-unitPositionFrom, pivot), 0.0)) 
                    * (1- clamp(depth / u_Thickness, 0.0, 1.0)) 
                    * (1- clamp(length(positionTo - positionFrom) / u_MaxDistance, 0.0, 1.0))
                    * (uv.x < 0.0 || uv.x > 1.0 ? 0.0 : 1.0)
                    * (uv.y < 0.0 || uv.y > 1.0 ? 0.0 : 1.0);

    visibility = clamp(visibility, 0.0, 1.0);
    uv.ba = vec2(visibility);
    v_FragColor = vec4(texture(u_ColorTex, uv.rg).rgb, 1.0);
}