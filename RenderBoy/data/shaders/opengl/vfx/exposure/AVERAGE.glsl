#SHADER COMPUTE
#version 460 core

layout (local_size_x = 256, local_size_y = 1) in;

#define EPSILON 0.005
#define RGB_TO_LUM vec3(0.2125, 0.7154, 0.0721)
#define MinLog2 -10.0
#define Log2Range 12.0

layout(std430, binding = 0) buffer Histogram
{
    uint histogram[256];
};

layout(std430, binding = 1) buffer HistoAvg
{
    float weightedAvgLum;
};

shared uint histogramShared[256];

uniform int u_PixelNum;
uniform float u_TimeDelta;

uint colorToBin(vec3 hdrColor, float minLogLum, float inverseLogLumRange);

void main() 
{
    uint localIndex = gl_LocalInvocationIndex;
    //  Get the count from the histogram buffer
    uint countForThisBin = histogram[localIndex];
    histogramShared[localIndex] = countForThisBin * localIndex;
    barrier();

    // Reset the count stored in the buffer in anticipation of the next pass
    histogram[localIndex] = 0;

    // This loop will perform a weighted count of the luminance range
    for (uint cutoff = (256 >> 1); cutoff > 0; cutoff >>= 1) 
    {
        if (uint(localIndex) < cutoff) 
        {
            histogramShared[localIndex] += histogramShared[localIndex + cutoff];
        }
        barrier();
    }

    // We only need to calculate this once, so only a single thread is needed.
    if (localIndex == 0) 
    {
        // Here we take our weighted sum and divide it by the number of pixels
        // that had luminance greater than zero (since the index == 0, we can
        // use countForThisBin to find the number of black pixels)
        float weightedLogAverage = (histogramShared[0] / max(u_PixelNum - float(countForThisBin), 1.0)) - 1.0;

        // Map from our histogram space to actual luminance
        float currentWeightedAvgLum = exp2(((weightedLogAverage / 254.0) * Log2Range) + MinLog2);
        float lastWeightedAvgLum = weightedAvgLum;
        weightedAvgLum = lastWeightedAvgLum + (currentWeightedAvgLum - lastWeightedAvgLum) * (1 - exp(-u_TimeDelta * 1.1));
    }
}