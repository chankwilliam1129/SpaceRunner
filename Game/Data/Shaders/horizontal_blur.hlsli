struct VS_OUT
{
	float4 position :SV_POSITION;
	float4 color : COLOR;
	float2 tex :TEXCOORD;
	float2 texcoord1 : TEXCOORD1;
	float2 texcoord2 : TEXCOORD2;
	float2 texcoord3 : TEXCOORD3;
	float2 texcoord4 : TEXCOORD4;
	float2 texcoord5 : TEXCOORD5;
	float2 texcoord6 : TEXCOORD6;
	float2 texcoord7 : TEXCOORD7;
	float2 texcoord8 : TEXCOORD8;
	float2 texcoord9 : TEXCOORD9;
};

cbuffer ConstantBuffer :register(b0)
{
	float screenWidth;
	float3 padding;
}