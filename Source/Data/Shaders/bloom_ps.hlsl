#include "mesh.hlsli"

//static const int bloomMapCount = 1;
#define bloomMapCount (3)

Texture2D diffuseMap : register(t0);
Texture2D bloomMap[bloomMapCount] : register(t1);
SamplerState diffuseMapSamplerState : register(s0);

cbuffer CbBloom : register(b4)
{
    float bloomStlength;
    float3 padding;
};

float4 main(VS_OUT pin) : SV_TARGET
{
    float3 color = 0;
    
    for (int i = 0; i < bloomMapCount; i++)
    {
        float3 blurTexture = bloomMap[i].Sample(diffuseMapSamplerState, pin.texcoord).xyz;
        color += blurTexture;
    }
    
    color = saturate(color * bloomStlength);
    
    return float4(color,1);
}