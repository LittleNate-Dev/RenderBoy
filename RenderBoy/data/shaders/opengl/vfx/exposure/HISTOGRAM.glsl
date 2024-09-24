#SHADER COMPUTE
#version 460 core

// Source: https://bruop.github.io/exposure/

layout (local_size_x = 32, local_size_y = 32) in;

#define EPSILON 0.005
#define RGB_TO_LUM vec3(0.2125, 0.7154, 0.0721)
#define MinLog2 -10.0
#define Log2Range 12.0
#define Log2RangeInverse 1.0/12.0

layout(rgba16f, binding = 0) uniform image2D u_Source;
layout(std430, binding = 1) buffer Buffer
{
    uint histogram[256];
};

shared uint histogramShared[256];

uint colorToBin(vec3 hdrColor);

void main() 
{
    // Initialize the bin for this thread to 0
    histogramShared[gl_LocalInvocationIndex] = 0;
    barrier();

    uvec2 dim = imageSize(u_Source).xy;
    // Ignore threads that map to areas beyond the bounds of our HDR image
    if (gl_GlobalInvocationID.x < dim.x && gl_GlobalInvocationID.y < dim.y) 
    {
        vec3 hdrColor = imageLoad(u_Source, ivec2(gl_GlobalInvocationID.xy)).xyz;
        uint binIndex = colorToBin(hdrColor);
        // We use an atomic add to ensure we don't write to the same bin in our
        // histogram from two different threads at the same time.
        atomicAdd(histogramShared[binIndex], 1);
    }

    barrier();
    atomicAdd(histogram[gl_LocalInvocationIndex], histogramShared[gl_LocalInvocationIndex]);
}

uint colorToBin(vec3 hdrColor) 
{
  // Convert our RGB value to Luminance, see note for RGB_TO_LUM macro above
  float lum = dot(hdrColor, RGB_TO_LUM);

  // Avoid taking the log of zero
  if (lum < EPSILON) 
  {
    return 0;
  }

  // Calculate the log_2 luminance and express it as a value in [0.0, 1.0]
  // where 0.0 represents the minimum luminance, and 1.0 represents the max.
  float logLum = clamp((log2(lum) - MinLog2) * Log2RangeInverse, 0.0, 1.0);

  // Map [0, 1] to [1, 255]. The zeroth bin is handled by the epsilon check above.
  return uint(logLum * 254.0 + 1.0);
}