#include "depth_stencil.h"

ID3D11DepthStencilState* DepthStencil::depthStencilState[DEPTH_MODE::DEPTH_MAX][STENCIL_MODE::STENCIL_MAX] = {};
ID3D11DeviceContext* DepthStencil::keepContext = nullptr;
DepthStencil::DEPTH_MODE   DepthStencil::nowDepthMode;
DepthStencil::STENCIL_MODE DepthStencil::nowStencilMode;

struct DepthData
{
	bool DepthEnable;
	D3D11_DEPTH_WRITE_MASK DepthWriteMask;
	D3D11_COMPARISON_FUNC DepthFunc;
};

struct StencilData
{
	bool StencilEnable;
	D3D11_STENCIL_OP      front_StencilFailOp;
	D3D11_STENCIL_OP      front_StencilDepthFailOp;
	D3D11_STENCIL_OP      front_StencilPassOp;
	D3D11_COMPARISON_FUNC fornt_StencilFunc;

	D3D11_STENCIL_OP      back_StencilFailOp;
	D3D11_STENCIL_OP      back_StencilDepthFailOp;
	D3D11_STENCIL_OP      back_StencilPassOp;
	D3D11_COMPARISON_FUNC back_StencilFunc;
};

DepthData depthData[DepthStencil::DEPTH_MODE::DEPTH_MAX] =
{
	//3D
	{
		true,							//default
		D3D11_DEPTH_WRITE_MASK_ALL,		//default
		D3D11_COMPARISON_LESS			//default
	},
	//2D
	{
		false,
		D3D11_DEPTH_WRITE_MASK_ALL,		//default
		D3D11_COMPARISON_LESS			//default
	},
};

StencilData stencilData[DepthStencil::STENCIL_MODE::STENCIL_MAX] = 
{
	//3D
	{
		true,
		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_STENCIL_OP_INCR,		
		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_COMPARISON_ALWAYS,	//default

		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_STENCIL_OP_DECR,
		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_COMPARISON_ALWAYS,	//default
	},
	//2D
	{
		false,
		D3D11_STENCIL_OP_KEEP,
		D3D11_STENCIL_OP_KEEP,
		D3D11_STENCIL_OP_KEEP,
		D3D11_COMPARISON_ALWAYS,

		D3D11_STENCIL_OP_KEEP,
		D3D11_STENCIL_OP_KEEP,
		D3D11_STENCIL_OP_KEEP,
		D3D11_COMPARISON_ALWAYS,
	},
	//MASK_INCR
	{
		true,
		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_STENCIL_OP_INCR_SAT,
		D3D11_COMPARISON_ALWAYS,	//default

		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_STENCIL_OP_INCR_SAT,
		D3D11_COMPARISON_ALWAYS,	//default
	},
	//MASK_DECR
	{
		true,
		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_STENCIL_OP_DECR_SAT,
		D3D11_COMPARISON_ALWAYS,	//default

		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_STENCIL_OP_DECR_SAT,
		D3D11_COMPARISON_ALWAYS,	//default
	},
	//MASK_APPLY
	{
		true,
		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_COMPARISON_LESS,	

		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_STENCIL_OP_KEEP,		//default
		D3D11_COMPARISON_LESS,
	},
};

void DepthStencil::Initialize(ID3D11Device* device, ID3D11DeviceContext* context) 
{
	for (int i = 0; i < DEPTH_MODE::DEPTH_MAX;i++) 
	{
		DepthData data = depthData[i];
		for (int j = 0; j < STENCIL_MODE::STENCIL_MAX;j++) 
		{
			StencilData data = stencilData[j];
			D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

			depthStencilDesc.DepthEnable    = depthData[i].DepthEnable;
			depthStencilDesc.DepthWriteMask = depthData[i].DepthWriteMask;
			depthStencilDesc.DepthFunc      = depthData[i].DepthFunc;

			depthStencilDesc.StencilEnable    = stencilData[j].StencilEnable;
			depthStencilDesc.StencilReadMask  = D3D11_DEFAULT_STENCIL_READ_MASK;
			depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

			depthStencilDesc.FrontFace.StencilFailOp      = stencilData[j].front_StencilFailOp;
			depthStencilDesc.FrontFace.StencilDepthFailOp = stencilData[j].front_StencilDepthFailOp;
			depthStencilDesc.FrontFace.StencilPassOp      = stencilData[j].front_StencilPassOp;
			depthStencilDesc.FrontFace.StencilFunc        = stencilData[j].fornt_StencilFunc;
														  
			depthStencilDesc.BackFace.StencilFailOp       = stencilData[j].back_StencilFailOp;
			depthStencilDesc.BackFace.StencilDepthFailOp  = stencilData[j].back_StencilDepthFailOp;
			depthStencilDesc.BackFace.StencilPassOp       = stencilData[j].back_StencilPassOp;
			depthStencilDesc.BackFace.StencilFunc         = stencilData[j].back_StencilFunc;

			device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState[i][j]);
		}
	}
	keepContext = context;
	nowDepthMode = DEPTH_MODE::DEPTH_MAX;
	nowStencilMode = STENCIL_MODE::STENCIL_MAX;
}

void DepthStencil::Set(DEPTH_MODE depthMode, STENCIL_MODE stencilMode)
{
	if (nowDepthMode == depthMode && nowStencilMode == stencilMode)return;
	keepContext->OMSetDepthStencilState(depthStencilState[depthMode][stencilMode], 1);
	nowDepthMode = depthMode;
	nowStencilMode = stencilMode;
}

void DepthStencil::Release()
{
	for (int i = 0; i < DEPTH_MODE::DEPTH_MAX;i++) 
	{
		for (int j = 0; j < STENCIL_MODE::STENCIL_MAX;j++) 
		{
			if(depthStencilState[i][j])depthStencilState[i][j]->Release();
			depthStencilState[i][j] = nullptr;
		}
	}
	keepContext = nullptr;
}