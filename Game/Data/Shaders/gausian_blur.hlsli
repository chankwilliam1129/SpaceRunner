struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

cbuffer ConstantBuffer :register(b0)
{
	int     SampleCount;
	float4  Offset[16];
}