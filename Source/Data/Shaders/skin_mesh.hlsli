struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color    : COLOR;
};

cbuffer CbCamera : register(b0)
{
	row_major float4x4	viewProjection;
};

cbuffer CbLight : register(b1)
{
	float4				lightDirection;
};

#define MAX_BONES 128
cbuffer CbMesh : register(b2)
{
	row_major float4x4	boneTransforms[MAX_BONES];
};

cbuffer CbSubset : register(b3)
{
	float4				materialColor;
};