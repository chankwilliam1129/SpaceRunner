#include "horizontal_blur.hlsli"
Texture2D shaderTexture;
SamplerState SampleType;

float4 main(VS_OUT pin) : SV_TARGET
{
	float weight0,weight1,weight2,weight3,weight4;
	float normalization;
	float4 color;

	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.55f;
	weight3 = 0.18f;
	weight4 = 0.1f;

	normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	color += shaderTexture.Sample(SampleType, pin.texcoord1) * weight4;
	color += shaderTexture.Sample(SampleType, pin.texcoord2) * weight3;
	color += shaderTexture.Sample(SampleType, pin.texcoord3) * weight2;
	color += shaderTexture.Sample(SampleType, pin.texcoord4) * weight1;
	color += shaderTexture.Sample(SampleType, pin.texcoord5) * weight0;
	color += shaderTexture.Sample(SampleType, pin.texcoord6) * weight1;
	color += shaderTexture.Sample(SampleType, pin.texcoord7) * weight2;
	color += shaderTexture.Sample(SampleType, pin.texcoord8) * weight3;
	color += shaderTexture.Sample(SampleType, pin.texcoord9) * weight4;

	color.a = 1.0f;

	return color;
}