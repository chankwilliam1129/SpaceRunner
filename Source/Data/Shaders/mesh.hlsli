struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color    : COLOR;
    float4 viewPosition : COLOR1;
};

cbuffer CbCamera : register(b0)
{
	row_major float4x4	viewProjection;
};

cbuffer CbLight : register(b1)
{
	float4				lightDirection;
};

cbuffer CbMesh : register(b2)
{
	row_major float4x4	world;
};

cbuffer CbSubset : register(b3)
{
	float4				materialColor;
};