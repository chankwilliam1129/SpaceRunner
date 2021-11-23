#include "mesh.hlsli"

VS_OUT main(
	float4 position : POSITION,
	float3 normal : NORMAL,
	float2 texcoord : TEXCOORD
)
{
    VS_OUT vout;
    float4 worldPosition = mul(position, world);
    float4 worldViewPosition = mul(worldPosition, viewProjection);
    vout.position = worldViewPosition;
    vout.viewPosition = vout.position;
    vout.color.xyzw = materialColor.xyzw;
    vout.texcoord = texcoord;

    return vout;
}