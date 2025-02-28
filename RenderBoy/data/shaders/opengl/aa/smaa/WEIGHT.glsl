#SHADER VERTEX
#version 460 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;

#define MAX_SEARCH_STEPS 16

out vec2 v_TexCoord;
out vec2 v_PixCoord;
out vec4 v_Offset[3];

uniform vec4 u_Metrics;

vec4 mad(vec4 a, vec4 b, vec4 c)
{
    return a * b + c;
}

void main()
{
    v_TexCoord = vec2((a_Position + 1.0) / 2.0);
    v_PixCoord = v_TexCoord * u_Metrics.zw;
    // We will use these offsets for the searches later on (see @PSEUDO_GATHER4):
    v_Offset[0] = mad(u_Metrics.xyxy, vec4(-0.25, -0.125,  1.25, -0.125), v_TexCoord.xyxy);
    v_Offset[1] = mad(u_Metrics.xyxy, vec4(-0.125, -0.25, -0.125,  1.25), v_TexCoord.xyxy);
    // And these for the searches, they indicate the ends of the loops:
    v_Offset[2] = mad(
        u_Metrics.xxyy,
        vec4(-2.0, 2.0, -2.0, 2.0) * float(MAX_SEARCH_STEPS),
        vec4(v_Offset[0].xz, v_Offset[1].yw)
    );

    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);   
} 



#SHADER FRAGMENT
#version 460 core

layout (location = 0) out vec4 v_FragColor;

in vec2 v_TexCoord;
in vec2 v_PixCoord;
in vec4 v_Offset[3];

precision highp float;
precision highp int;

// Defaults
#define THRESHOLD 0.1
#define MAX_SEARCH_STEPS 32
#define MAX_SEARCH_STEPS_DIAG 16
#define CORNER_ROUNDING 25
// Non-Configurable Defines
#define AREATEX_MAX_DISTANCE 16
#define AREATEX_MAX_DISTANCE_DIAG 20
#define AREATEX_PIXEL_SIZE (1.0 / vec2(160.0, 560.0))
#define AREATEX_SUBTEX_SIZE (1.0 / 7.0)
#define SEARCHTEX_SIZE vec2(66.0, 33.0)
#define SEARCHTEX_PACKED_SIZE vec2(64.0, 16.0)
#define CORNER_ROUNDING_NORM (float(CORNER_ROUNDING) / 100.0)
// Texture Access Defines
#define AREATEX_SELECT(sample) sample.rg
#define SEARCHTEX_SELECT(sample) sample.r

uniform sampler2D u_EdgeTex;
uniform sampler2D u_AreaTex;
uniform sampler2D u_SearchTex;
uniform vec4 u_Metrics;

#define mad(a, b, c) (a * b + c)
#define saturate(a) clamp(a, 0.0, 1.0)
#define round(v) floor(v + 0.5)
#define SampleLevelZeroOffset(tex, coord, offset) texture2D(tex, coord + offset * u_Metrics.xy)

/**
 * Conditional move:
 */
void Movc(bvec2 cond, inout vec2 variable, vec2 value)
{
    if (cond.x)
    {
        variable.x = value.x;
    }
  
    if (cond.y)
    {
        variable.y = value.y;
    }
}

void Movc(bvec4 cond, inout vec4 variable, vec4 value) 
{
    Movc(cond.xy, variable.xy, value.xy);
    Movc(cond.zw, variable.zw, value.zw);
}

/**
 * Allows to decode two binary values from a bilinear-filtered access.
 */
vec2 DecodeDiagBilinearAccess(vec2 e) 
{
    // Bilinear access for fetching 'e' have a 0.25 offset, and we are
    // interested in the R and G edges:
    //
    // +---G---+-------+
    // |   x o R   x   |
    // +-------+-------+
    //
    // Then, if one of these edge is enabled:
    //   Red:   (0.75 * X + 0.25 * 1) => 0.25 or 1.0
    //   Green: (0.75 * 1 + 0.25 * X) => 0.75 or 1.0
    //
    // This function will unpack the values (mad + mul + round):
    // wolframalpha.com: round(x * abs(5 * x - 5 * 0.75)) plot 0 to 1
    e.r = e.r * abs(5.0 * e.r - 5.0 * 0.75);
    return round(e);
}

vec4 DecodeDiagBilinearAccess(vec4 e) 
{
    e.rb = e.rb * abs(5.0 * e.rb - 5.0 * 0.75);
    return round(e);
}

/**
 * These functions allows to perform diagonal pattern searches.
 */
vec2 SearchDiag1(sampler2D edgesTex, vec2 texcoord, vec2 dir, out vec2 e) 
{
    vec4 coord = vec4(texcoord, -1.0, 1.0);
    vec3 t = vec3(u_Metrics.xy, 1.0);

    for (int i = 0; i < MAX_SEARCH_STEPS; i++) 
    {
        if (!(coord.z < float(MAX_SEARCH_STEPS_DIAG - 1) && coord.w > 0.9)) 
        {
            break;
        }
        coord.xyz = mad(t, vec3(dir, 1.0), coord.xyz);
        e = texture2D(edgesTex, coord.xy).rg; // LinearSampler
        coord.w = dot(e, vec2(0.5, 0.5));
    }
    return coord.zw;
}

vec2 SearchDiag2(sampler2D edgesTex, vec2 texcoord, vec2 dir, out vec2 e) 
{
    vec4 coord = vec4(texcoord, -1.0, 1.0);
    coord.x += 0.25 * u_Metrics.x; // See @SearchDiag2Optimization
    vec3 t = vec3(u_Metrics.xy, 1.0);
    for (int i = 0; i < MAX_SEARCH_STEPS; i++) 
    {
        if (!(coord.z < float(MAX_SEARCH_STEPS_DIAG - 1) && coord.w > 0.9))
        {
            break;
        }
        coord.xyz = mad(t, vec3(dir, 1.0), coord.xyz);
        // @SearchDiag2Optimization
        // Fetch both edges at once using bilinear filtering:
        e = texture2D(edgesTex, coord.xy).rg; // LinearSampler
        e = DecodeDiagBilinearAccess(e);
        // Non-optimized version:
        // e.g = texture2D(edgesTex, coord.xy).g; // LinearSampler
        // e.r = SMAASampleLevelZeroOffset(edgesTex, coord.xy, vec2(1, 0)).r;
        coord.w = dot(e, vec2(0.5, 0.5));
    }
    return coord.zw;
}

/**
 * Similar to SMAAArea, this calculates the area corresponding to a certain
 * diagonal distance and crossing edges 'e'.
 */
vec2 AreaDiag(sampler2D areaTex, vec2 dist, vec2 e, float offset) 
{
  vec2 texcoord = mad(vec2(AREATEX_MAX_DISTANCE_DIAG, AREATEX_MAX_DISTANCE_DIAG), e, dist);

  // We do a scale and bias for mapping to texel space:
  texcoord = mad(AREATEX_PIXEL_SIZE, texcoord, 0.5 * AREATEX_PIXEL_SIZE);

  // Diagonal areas are on the second half of the texture:
  texcoord.x += 0.5;

  // Move to proper place, according to the subpixel offset:
  texcoord.y += AREATEX_SUBTEX_SIZE * offset;

  // Do it!
  return AREATEX_SELECT(texture2D(areaTex, texcoord)); // LinearSampler
}

/**
 * This searches for diagonal patterns and returns the corresponding weights.
 */
vec2 CalculateDiagWeights(sampler2D edgesTex, sampler2D areaTex, vec2 texcoord, vec2 e, vec4 subsampleIndices) 
{
    vec2 weights = vec2(0.0, 0.0);
    // Search for the line ends:
    vec4 d;
    vec2 end;
    if (e.r > 0.0) 
    {
        d.xz = SearchDiag1(edgesTex, texcoord, vec2(-1.0,  1.0), end);
        d.x += float(end.y > 0.9);
    } 
    else
    {
        d.xz = vec2(0.0, 0.0);
    }    
    d.yw = SearchDiag1(edgesTex, texcoord, vec2(1.0, -1.0), end);

    if (d.x + d.y > 2.0) // d.x + d.y + 1 > 3
    { 
        // Fetch the crossing edges:
        vec4 coords = mad(vec4(-d.x + 0.25, d.x, d.y, -d.y - 0.25), u_Metrics.xyxy, texcoord.xyxy);
        vec4 c;
        c.xy = SampleLevelZeroOffset(edgesTex, coords.xy, vec2(-1,  0)).rg;
        c.zw = SampleLevelZeroOffset(edgesTex, coords.zw, vec2( 1,  0)).rg;
        c.yxwz = DecodeDiagBilinearAccess(c.xyzw);

        // Non-optimized version:
        // vec4 coords = mad(vec4(-d.x, d.x, d.y, -d.y), SMAA_RT_METRICS.xyxy, texcoord.xyxy);
        // vec4 c;
        // c.x = SMAASampleLevelZeroOffset(edgesTex, coords.xy, vec2(-1,  0)).g;
        // c.y = SMAASampleLevelZeroOffset(edgesTex, coords.xy, vec2( 0,  0)).r;
        // c.z = SMAASampleLevelZeroOffset(edgesTex, coords.zw, vec2( 1,  0)).g;
        // c.w = SMAASampleLevelZeroOffset(edgesTex, coords.zw, vec2( 1, -1)).r;

        // Merge crossing edges at each side into a single value:
        vec2 cc = mad(vec2(2.0, 2.0), c.xz, c.yw);
        // Remove the crossing edge if we didn't found the end of the line:
        Movc(bvec2(step(0.9, d.zw)), cc, vec2(0.0, 0.0));
        // Fetch the areas for this line:
        weights += AreaDiag(areaTex, d.xy, cc, subsampleIndices.z);
    }

    // Search for the line ends:
    d.xz = SearchDiag2(edgesTex, texcoord, vec2(-1.0, -1.0), end);
    if (SampleLevelZeroOffset(edgesTex, texcoord, vec2(1, 0)).r > 0.0) 
    {
        d.yw = SearchDiag2(edgesTex, texcoord, vec2(1.0, 1.0), end);
        d.y += float(end.y > 0.9);
    } 
    else 
    {
        d.yw = vec2(0.0, 0.0);
    }
    if (d.x + d.y > 2.0) // d.x + d.y + 1 > 3
    { 
        // Fetch the crossing edges:
        vec4 coords = mad(vec4(-d.x, -d.x, d.y, d.y), u_Metrics.xyxy, texcoord.xyxy);
        vec4 c;
        c.x  = SampleLevelZeroOffset(edgesTex, coords.xy, vec2(-1,  0)).g;
        c.y  = SampleLevelZeroOffset(edgesTex, coords.xy, vec2( 0, -1)).r;
        c.zw = SampleLevelZeroOffset(edgesTex, coords.zw, vec2( 1,  0)).gr;
        vec2 cc = mad(vec2(2.0, 2.0), c.xz, c.yw);
        // Remove the crossing edge if we didn't found the end of the line:
        Movc(bvec2(step(0.9, d.zw)), cc, vec2(0.0, 0.0));
        // Fetch the areas for this line:
        weights += AreaDiag(areaTex, d.xy, cc, subsampleIndices.w).gr;
    }
    return weights;
}

/**
 * This allows to determine how much length should we add in the last step
 * of the searches. It takes the bilinearly interpolated edge (see
 * @PSEUDO_GATHER4), and adds 0, 1 or 2, depending on which edges and
 * crossing edges are active.
 */
float SearchLength(sampler2D searchTex, vec2 e, float offset) 
{
    // The texture is flipped vertically, with left and right cases taking half
    // of the space horizontally:
    vec2 scale = SEARCHTEX_SIZE * vec2(0.5, -1.0);
    vec2 bias = SEARCHTEX_SIZE * vec2(offset, 1.0);
    // Scale and bias to access texel centers:
    scale += vec2(-1.0,  1.0);
    bias  += vec2( 0.5, -0.5);
    // Convert from pixel coordinates to texcoords:
    // (We use SMAA_SEARCHTEX_PACKED_SIZE because the texture is cropped)
    scale *= 1.0 / SEARCHTEX_PACKED_SIZE;
    bias *= 1.0 / SEARCHTEX_PACKED_SIZE;
    // Lookup the search texture:
    return SEARCHTEX_SELECT(texture2D(searchTex, mad(scale, e, bias))); // LinearSampler
}

/**
 * Horizontal/vertical search functions for the 2nd pass.
 */
float SearchXLeft(sampler2D edgesTex, sampler2D searchTex, vec2 texcoord, float end) 
{
  /**
    * @PSEUDO_GATHER4
    * This texcoord has been offset by (-0.25, -0.125) in the vertex shader to
    * sample between edge, thus fetching four edges in a row.
    * Sampling with different offsets in each direction allows to disambiguate
    * which edges are active from the four fetched ones.
    */
    vec2 e = vec2(0.0, 1.0);
    for (int i = 0; i < MAX_SEARCH_STEPS; i++) 
    {
        if (!(texcoord.x > end && e.g > 0.8281 && e.r == 0.0)) break;
        e = texture2D(edgesTex, texcoord).rg; // LinearSampler
        texcoord = mad(-vec2(2.0, 0.0), u_Metrics.xy, texcoord);
    }
    float offset = mad(-(255.0 / 127.0), SearchLength(searchTex, e, 0.0), 3.25);
    return mad(u_Metrics.x, offset, texcoord.x);

    // Non-optimized version:
    // We correct the previous (-0.25, -0.125) offset we applied:
    // texcoord.x += 0.25 * SMAA_RT_METRICS.x;

    // The searches are bias by 1, so adjust the coords accordingly:
    // texcoord.x += SMAA_RT_METRICS.x;

    // Disambiguate the length added by the last step:
    // texcoord.x += 2.0 * SMAA_RT_METRICS.x; // Undo last step
    // texcoord.x -= SMAA_RT_METRICS.x * (255.0 / 127.0) * SMAASearchLength(searchTex, e, 0.0);
    // return mad(SMAA_RT_METRICS.x, offset, texcoord.x);
}

float SearchXRight(sampler2D edgesTex, sampler2D searchTex, vec2 texcoord, float end) 
{
    vec2 e = vec2(0.0, 1.0);
    for (int i = 0; i < MAX_SEARCH_STEPS; i++) 
    { 
        if (!(texcoord.x < end && e.g > 0.8281 && e.r == 0.0))
        {
            break;
        }
        e = texture2D(edgesTex, texcoord).rg; // LinearSampler
        texcoord = mad(vec2(2.0, 0.0), u_Metrics.xy, texcoord);
    }
    float offset = mad(-(255.0 / 127.0), SearchLength(searchTex, e, 0.5), 3.25);
    return mad(-u_Metrics.x, offset, texcoord.x);
}

float SearchYUp(sampler2D edgesTex, sampler2D searchTex, vec2 texcoord, float end) 
{
    vec2 e = vec2(1.0, 0.0);
    for (int i = 0; i < MAX_SEARCH_STEPS; i++) 
    { 
        if (!(texcoord.y > end && e.r > 0.8281 && e.g == 0.0))
        {
            break;
        }
        e = texture2D(edgesTex, texcoord).rg; // LinearSampler
        texcoord = mad(-vec2(0.0, 2.0), u_Metrics.xy, texcoord);
    }
    float offset = mad(-(255.0 / 127.0), SearchLength(searchTex, e.gr, 0.0), 3.25);
    return mad(u_Metrics.y, offset, texcoord.y);
}

float SearchYDown(sampler2D edgesTex, sampler2D searchTex, vec2 texcoord, float end) 
{
    vec2 e = vec2(1.0, 0.0);
    for (int i = 0; i < MAX_SEARCH_STEPS; i++) 
    { 
        if (!(texcoord.y < end && e.r > 0.8281 && e.g == 0.0))
        {
            break;
        }
        e = texture2D(edgesTex, texcoord).rg; // LinearSampler
        texcoord = mad(vec2(0.0, 2.0), u_Metrics.xy, texcoord);
    }
    float offset = mad(-(255.0 / 127.0), SearchLength(searchTex, e.gr, 0.5), 3.25);
    return mad(-u_Metrics.y, offset, texcoord.y);
}

/**
 * Ok, we have the distance and both crossing edges. So, what are the areas
 * at each side of current edge?
 */
vec2 Area(sampler2D areaTex, vec2 dist, float e1, float e2, float offset) 
{
    // Rounding prevents precision errors of bilinear filtering:
    vec2 texcoord = mad(vec2(AREATEX_MAX_DISTANCE, AREATEX_MAX_DISTANCE), round(4.0 * vec2(e1, e2)), dist);
    // We do a scale and bias for mapping to texel space:
    texcoord = mad(AREATEX_PIXEL_SIZE, texcoord, 0.5 * AREATEX_PIXEL_SIZE);
    // Move to proper place, according to the subpixel offset:
    texcoord.y = mad(AREATEX_SUBTEX_SIZE, offset, texcoord.y);
    // Do it!
    return AREATEX_SELECT(texture2D(areaTex, texcoord)); // LinearSampler
}

// Corner Detection Functions
void DetectHorizontalCornerPattern(sampler2D edgesTex, inout vec2 weights, vec4 texcoord, vec2 d) 
{
    // #if !defined(SMAA_DISABLE_CORNER_DETECTION)
    vec2 leftRight = step(d.xy, d.yx);
    vec2 rounding = (1.0 - CORNER_ROUNDING_NORM) * leftRight;

    rounding /= leftRight.x + leftRight.y; // Reduce blending for pixels in the center of a line.

    vec2 factor = vec2(1.0, 1.0);
    factor.x -= rounding.x * SampleLevelZeroOffset(edgesTex, texcoord.xy, vec2(0,  1)).r;
    factor.x -= rounding.y * SampleLevelZeroOffset(edgesTex, texcoord.zw, vec2(1,  1)).r;
    factor.y -= rounding.x * SampleLevelZeroOffset(edgesTex, texcoord.xy, vec2(0, -2)).r;
    factor.y -= rounding.y * SampleLevelZeroOffset(edgesTex, texcoord.zw, vec2(1, -2)).r;

    weights *= saturate(factor);
    // #endif
}

void DetectVerticalCornerPattern(sampler2D edgesTex, inout vec2 weights, vec4 texcoord, vec2 d) 
{
    // #if !defined(SMAA_DISABLE_CORNER_DETECTION)
    vec2 leftRight = step(d.xy, d.yx);
    vec2 rounding = (1.0 - CORNER_ROUNDING_NORM) * leftRight;

    rounding /= leftRight.x + leftRight.y;

    vec2 factor = vec2(1.0, 1.0);
    factor.x -= rounding.x * SampleLevelZeroOffset(edgesTex, texcoord.xy, vec2( 1, 0)).g;
    factor.x -= rounding.y * SampleLevelZeroOffset(edgesTex, texcoord.zw, vec2( 1, 1)).g;
    factor.y -= rounding.x * SampleLevelZeroOffset(edgesTex, texcoord.xy, vec2(-2, 0)).g;
    factor.y -= rounding.y * SampleLevelZeroOffset(edgesTex, texcoord.zw, vec2(-2, 1)).g;

    weights *= saturate(factor);
    // #endif
}

void main() 
{
    vec4 subsampleIndices = vec4(0.0); // Just pass zero for SMAA 1x, see @SUBSAMPLE_INDICES.
    // subsampleIndices = vec4(1.0, 1.0, 1.0, 0.0);
    vec4 weights = vec4(0.0);
    vec2 e = texture2D(u_EdgeTex, v_TexCoord).rg;
    if (e.g > 0.0) // Edge at north
    { 
        // #if !defined(SMAA_DISABLE_DIAG_DETECTION)
        // Diagonals have both north and west edges, so searching for them in
        // one of the boundaries is enough.
        weights.rg = CalculateDiagWeights(u_EdgeTex, u_AreaTex, v_TexCoord, e, subsampleIndices);
        // We give priority to diagonals, so if we find a diagonal we skip
        // horizontal/vertical processing.
        if (weights.r == -weights.g) // weights.r + weights.g == 0.0
        { 
        // #endif
        vec2 d;
        // Find the distance to the left:
        vec3 coords;
        coords.x = SearchXLeft(u_EdgeTex, u_SearchTex, v_Offset[0].xy, v_Offset[2].x);
        coords.y = v_Offset[1].y; // vOffset[1].y = vTexCoord0.y - 0.25 * SMAA_RT_METRICS.y (@CROSSING_OFFSET)
        d.x = coords.x;
        // Now fetch the left crossing edges, two at a time using bilinear
        // filtering. Sampling at -0.25 (see @CROSSING_OFFSET) enables to
        // discern what value each edge has:
        float e1 = texture2D(u_EdgeTex, coords.xy).r; // LinearSampler
        // Find the distance to the right:
        coords.z = SearchXRight(u_EdgeTex, u_SearchTex, v_Offset[0].zw, v_Offset[2].y);
        d.y = coords.z;
        // We want the distances to be in pixel units (doing this here allow to
        // better interleave arithmetic and memory accesses):
        d = abs(round(mad(u_Metrics.zz, d, -v_PixCoord.xx)));
        // SMAAArea below needs a sqrt, as the areas texture is compressed
        // quadratically:
        vec2 sqrt_d = sqrt(d);
        // Fetch the right crossing edges:
        float e2 = SampleLevelZeroOffset(u_EdgeTex, coords.zy, vec2(1, 0)).r;
        // Ok, we know how this pattern looks like, now it is time for getting
        // the actual area:
        weights.rg = Area(u_AreaTex, sqrt_d, e1, e2, subsampleIndices.y);
        // Fix corners:
        coords.y = v_TexCoord.y;
        DetectHorizontalCornerPattern(u_EdgeTex, weights.rg, coords.xyzy, d);
        // #if !defined(SMAA_DISABLE_DIAG_DETECTION)
        } 
        else
        {
            e.r = 0.0; // Skip vertical processing.
        }
        // #endif
    }

    if (e.r > 0.0) // Edge at west
    {
        vec2 d;
        // Find the distance to the top:
        vec3 coords;
        coords.y = SearchYUp(u_EdgeTex, u_SearchTex, v_Offset[1].xy, v_Offset[2].z);
        coords.x = v_Offset[0].x; // vOffset[1].x = vTexCoord0.x - 0.25 * SMAA_RT_METRICS.x;
        d.x = coords.y;

        // Fetch the top crossing edges:
        float e1 = texture2D(u_EdgeTex, coords.xy).g; // LinearSampler

        // Find the distance to the bottom:
        coords.z = SearchYDown(u_EdgeTex, u_SearchTex, v_Offset[1].zw, v_Offset[2].w);
        d.y = coords.z;

        // We want the distances to be in pixel units:
        d = abs(round(mad(u_Metrics.ww, d, -v_PixCoord.yy)));

        // SMAAArea below needs a sqrt, as the areas texture is compressed
        // quadratically:
        vec2 sqrt_d = sqrt(d);

        // Fetch the bottom crossing edges:
        float e2 = SampleLevelZeroOffset(u_EdgeTex, coords.xz, vec2(0, 1)).g;

        // Get the area for this direction:
        weights.ba = Area(u_AreaTex, sqrt_d, e1, e2, subsampleIndices.x);

        // Fix corners:
        coords.x = v_TexCoord.x;
        DetectVerticalCornerPattern(u_EdgeTex, weights.ba, coords.xyxz, d);
    }
    v_FragColor = weights;
}