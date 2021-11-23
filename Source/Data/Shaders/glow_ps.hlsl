#include "mesh.hlsli"

Texture2D diffuseMap : register(t0);
Texture2D glowMap : register(t1);
SamplerState diffuseMapSamplerState : register(s0);

cbuffer CbGlow : register(b4)
{
    float glowStrength;
    float3 padding;
};

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 textureColor = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord);

    float4 glowColor = glowMap.Sample(diffuseMapSamplerState, pin.texcoord);

    float4 color = saturate(textureColor + (glowColor * glowStrength));

    return color;
}