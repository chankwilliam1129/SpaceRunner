#include "horizontal_blur.hlsli"

VS_OUT main(float4 position : POSITION, float4 color : COLOR, float2 tex : TEXCOORD)
{
	VS_OUT vout;
	vout.position = position;
	vout.color = color;
	vout.tex = tex;
	float texelSize = 1.0f / screenWidth;

	vout.texcoord1 = tex + float2(texelSize * -4.0f, 0.0f);
	vout.texcoord2 = tex + float2(texelSize * -3.0f, 0.0f);
	vout.texcoord3 = tex + float2(texelSize * -2.0f, 0.0f);
	vout.texcoord4 = tex + float2(texelSize * -1.0f, 0.0f);
	vout.texcoord5 = tex + float2(texelSize * +0.0f, 0.0f);
	vout.texcoord6 = tex + float2(texelSize * +1.0f, 0.0f);
	vout.texcoord7 = tex + float2(texelSize * +2.0f, 0.0f);
	vout.texcoord8 = tex + float2(texelSize * +3.0f, 0.0f);
	vout.texcoord9 = tex + float2(texelSize * +4.0f, 0.0f);

	return vout;
}