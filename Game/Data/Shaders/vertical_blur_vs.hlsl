#include "vertical_blur.hlsli"

VS_OUT main(float4 position : POSITION, float4 color : COLOR, float2 tex : TEXCOORD)
{
	VS_OUT vout;
	vout.position = position;
	vout.color = color;
	vout.tex = tex;
	float texelSize = 1.0f / screenHeight;

	vout.texcoord1 = tex + float2(0.0f, texelSize * -4.0f);
	vout.texcoord2 = tex + float2(0.0f, texelSize * -3.0f);
	vout.texcoord3 = tex + float2(0.0f, texelSize * -2.0f);
	vout.texcoord4 = tex + float2(0.0f, texelSize * -1.0f);
	vout.texcoord5 = tex + float2(0.0f, texelSize * 0.0f);
	vout.texcoord6 = tex + float2(0.0f, texelSize * 1.0f);
	vout.texcoord7 = tex + float2(0.0f, texelSize * 2.0f);
	vout.texcoord8 = tex + float2(0.0f, texelSize * 3.0f);
	vout.texcoord9 = tex + float2(0.0f, texelSize * 4.0f);

	return vout;
}