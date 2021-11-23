#include "gausian_blur.hlsli"

Texture2D       ColorBuffer  : register(t0);
SamplerState    ColorSampler : register(s0);

float4  main(VS_OUT pin) : SV_TARGET
{
    float4 result = 0;
    for (int i = 0; i < SampleCount; ++i)
    {
        result += Offset[i].z * ColorBuffer.Sample(ColorSampler, pin.texcoord + Offset[i].xy);
    }
    result.w = 1.0f;
    return result;
}