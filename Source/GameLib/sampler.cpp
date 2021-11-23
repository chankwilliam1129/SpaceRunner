#include "sampler.h"

ID3D11SamplerState*  Sampler::samplerState[MODE_MAX] = {0};
ID3D11DeviceContext* Sampler::keepContext = nullptr;

struct SamplerData 
{
	D3D11_FILTER filter;
	D3D11_TEXTURE_ADDRESS_MODE addressU;
	D3D11_TEXTURE_ADDRESS_MODE addressV;
	D3D11_TEXTURE_ADDRESS_MODE addressW;
	FLOAT mipLODBias;
	UINT  maxAnisotropy;
	D3D11_COMPARISON_FUNC comparisonFunc;
	FLOAT borderColor[4];
	FLOAT minLOD;
	FLOAT maxLOD;
};

SamplerData sampleData[Sampler::MODE_MAX] =
{
	//NONE
	{

	},
	//SPRITE
	{
		D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_BORDER,
		D3D11_TEXTURE_ADDRESS_BORDER,
		D3D11_TEXTURE_ADDRESS_BORDER,
		0,
		16,
		D3D11_COMPARISON_ALWAYS,
		{1.0f,1.0f,1.0f,1.0f},
		0,
		D3D11_FLOAT32_MAX,
	},
	//WRAP
	{			
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		1,
		D3D11_COMPARISON_ALWAYS,
		{0,0,0,0},
		0,
		D3D11_FLOAT32_MAX,
	},
	//CLAMP
	{
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		0.0f,
		1,
		D3D11_COMPARISON_ALWAYS,
		{0,0,0,0},
		0,
		D3D11_FLOAT32_MAX,
	},
};

void Sampler::Initialize(ID3D11Device* device, ID3D11DeviceContext* context) 
{
	for (int i = 0; i < MODE_MAX; i++)
	{
		SamplerData* data = sampleData;
		D3D11_SAMPLER_DESC	samplerDesc = {};
		samplerDesc.Filter = data[i].filter;

		samplerDesc.AddressU = data[i].addressU;
		samplerDesc.AddressV = data[i].addressV;
		samplerDesc.AddressW = data[i].addressW;

		samplerDesc.MipLODBias     = data[i].mipLODBias;
		samplerDesc.MaxAnisotropy  = data[i].maxAnisotropy;
		samplerDesc.ComparisonFunc = data[i].comparisonFunc;

		for (int num = 0; num < 4; num++)
		{
			samplerDesc.BorderColor[num] = data[i].borderColor[num];
		}
		samplerDesc.MinLOD = data[i].minLOD;
		samplerDesc.MaxLOD = data[i].maxLOD;
		device->CreateSamplerState(&samplerDesc, &samplerState[i]);
	}
	keepContext = context;
}

void Sampler::Set(Sampler::MODE mode)
{
	keepContext->PSSetSamplers(0, 1, &samplerState[mode]);
}

void Sampler::Release()
{
	for (auto& it : samplerState) 
	{
		if (it) { it->Release(); }
		it = nullptr;
	}
	keepContext = nullptr;
}