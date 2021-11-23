#include "rasterizer.h"

ID3D11RasterizerState* Rasterizer::rasterizerState[MODE_MAX] = { 0 };
ID3D11DeviceContext*   Rasterizer::keepContext = nullptr;

struct RasterizerData
{
	D3D11_FILL_MODE fillMode;
	D3D11_CULL_MODE cullMode;
	bool frontCounterClockwise;
	bool depthClipEnable;
	bool antialiasedLineEnable;
};

RasterizerData rasterizerData[Rasterizer::MODE_MAX] =
{
	//DEFAULT
	{
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK,
		false,
		true,
		false,
	},
	//SPRITE
	{
		D3D11_FILL_SOLID,
		D3D11_CULL_NONE,
		false,
		false,
		false,
	},
	//WIREFLAME
	{
		D3D11_FILL_WIREFRAME,
		D3D11_CULL_BACK,
		false,
		true,
		true,
	},
	//SOLID
	{
		D3D11_FILL_SOLID,
		D3D11_CULL_NONE,
		true,
		true,
		false,
	},
};

void Rasterizer::Initialize(ID3D11Device* device, ID3D11DeviceContext* context) 
{
	HRESULT	hr = S_OK;
	for (int i = 0;i < MODE_MAX;i++) 
	{
		RasterizerData &data = rasterizerData[i];
		D3D11_RASTERIZER_DESC rsDesc = {};
		rsDesc.FillMode              = data.fillMode;
		rsDesc.CullMode              = data.cullMode;
		rsDesc.FrontCounterClockwise = data.frontCounterClockwise;
		rsDesc.DepthClipEnable       = data.depthClipEnable;
		rsDesc.AntialiasedLineEnable = data.antialiasedLineEnable;

		hr = device->CreateRasterizerState(&rsDesc, &rasterizerState[i]);
	}
	keepContext = context;
}

void Rasterizer::Set(Rasterizer::MODE mode)
{
	keepContext->RSSetState(rasterizerState[mode]);
}

void Rasterizer::Release()
{
	for (auto& it : rasterizerState) 
	{
		it->Release();
		it = nullptr;
	}
	keepContext = nullptr;
}