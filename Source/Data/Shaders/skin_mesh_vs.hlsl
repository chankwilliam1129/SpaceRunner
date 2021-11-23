#include "skin_mesh.hlsli"

VS_OUT main(
	float4 position : POSITION,
	float3 normal : NORMAL,
	float2 texcoord : TEXCOORD,
	float4 boneWeights : WEIGHTS,
	uint4  boneIndices : BONES
)
{
	float3 p = { 0, 0, 0 };
	float3 n = { 0, 0, 0 };
	for (int i = 0; i < 4; i++)
	{
		p += (boneWeights[i] * mul(position, boneTransforms[boneIndices[i]])).xyz;
		n += (boneWeights[i] * mul(float4(normal.xyz, 0), boneTransforms[boneIndices[i]])).xyz;
	}

	VS_OUT vout;
	vout.position = mul(float4(p, 1.0f), viewProjection);

	float3 N = normalize(n);
	float3 L = normalize((float3) - lightDirection);
	float d = dot(L, N);
	vout.color.xyz = materialColor.xyz * max(0, d);
	vout.color.w = materialColor.w;
	vout.texcoord = texcoord;

	return vout;
}
