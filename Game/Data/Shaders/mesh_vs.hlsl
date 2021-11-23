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
	float3 N = normalize(mul(float4(normal, 0), world));
	float3 L = normalize((float3) - lightDirection);
	float d = dot(L, N);
	vout.color.xyz = materialColor.xyz * max(0, d * 0.65f + 0.35f);
	vout.color.w = materialColor.w;
	vout.texcoord = texcoord;

	return vout;
}