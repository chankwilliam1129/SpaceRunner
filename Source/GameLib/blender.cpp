#include <assert.h>
#include "blender.h"

ID3D11BlendState*		Blender::blendState[MODE_MAX] = {0};
bool					Blender::bLoad = false;
Blender::BLEND_MODE		Blender::nowMode;
ID3D11DeviceContext*    Blender::keepContext = nullptr;

struct _blend_data
{
	D3D11_BLEND		SrcBlend;
	D3D11_BLEND		DestBlend;
	D3D11_BLEND_OP	BlendOp;
	D3D11_BLEND		SrcBlendAlpha;
	D3D11_BLEND		DestBlendAlpha;
	D3D11_BLEND_OP	BlendOpAlpha;
}
blend_data[Blender::MODE_MAX] =
{
	{	//	NONE
		D3D11_BLEND_ONE,				//	SrcBlend
		D3D11_BLEND_ZERO,				//	DestBlend
		D3D11_BLEND_OP_ADD,				//	BlendOp
		D3D11_BLEND_ONE,				//	SrcBlendAlpha
		D3D11_BLEND_ZERO,				//	DestBlendAlpha
		D3D11_BLEND_OP_ADD				//	BlendOpAlpha
	},	//	NONE
	{	//	ALPHA
		D3D11_BLEND_SRC_ALPHA,			//	SrcBlend
		D3D11_BLEND_INV_SRC_ALPHA,		//	DestBlend
		D3D11_BLEND_OP_ADD,				//	BlendOp
		D3D11_BLEND_ONE,				//	SrcBlendAlpha
		D3D11_BLEND_ZERO,				//	DestBlendAlpha
		D3D11_BLEND_OP_ADD				//	BlendOpAlpha
	},	//	ALPHA
	{	//	ADD
		D3D11_BLEND_SRC_ALPHA,			//	SrcBlend
		D3D11_BLEND_ONE,				//	DestBlend
		D3D11_BLEND_OP_ADD,				//	BlendOp
		D3D11_BLEND_ONE,				//	SrcBlendAlpha
		D3D11_BLEND_ZERO,				//	DestBlendAlpha
		D3D11_BLEND_OP_ADD,				//	BlendOpAlpha
	},	//	ADD
	{	//	SUB
		D3D11_BLEND_SRC_ALPHA,			//	SrcBlend
		D3D11_BLEND_ONE,				//	DestBlend
		D3D11_BLEND_OP_REV_SUBTRACT,	//	BlendOp
		D3D11_BLEND_ONE,				//	SrcBlendAlpha
		D3D11_BLEND_ZERO,				//	DestBlendAlpha
		D3D11_BLEND_OP_ADD,				//	BlendOpAlpha
	},	//	SUB
	{	//	REPLACE
		D3D11_BLEND_SRC_ALPHA,			//	SrcBlend
		D3D11_BLEND_ZERO,				//	DestBlend
		D3D11_BLEND_OP_ADD,				//	BlendOp
		D3D11_BLEND_ONE,				//	SrcBlendAlpha
		D3D11_BLEND_ZERO,				//	DestBlendAlpha
		D3D11_BLEND_OP_ADD				//	BlendOpAlpha
	},	//	REPLACE
	{	//	MULTIPLY
		D3D11_BLEND_ZERO,				//	SrcBlend
		D3D11_BLEND_SRC_COLOR,			//	DestBlend
		D3D11_BLEND_OP_ADD,				//	BlendOp
		D3D11_BLEND_DEST_ALPHA,			//	SrcBlendAlpha
		D3D11_BLEND_ZERO,				//	DestBlendAlpha
		D3D11_BLEND_OP_ADD				//	BlendOpAlpha
	},	//	MULTIPLY
	{	//	LIGHTEN
		D3D11_BLEND_ONE,				//	SrcBlend
		D3D11_BLEND_ONE,				//	DestBlend
		D3D11_BLEND_OP_MAX,				//	BlendOp
		D3D11_BLEND_ONE,				//	SrcBlendAlpha
		D3D11_BLEND_ONE,				//	DestBlendAlpha
		D3D11_BLEND_OP_MAX				//	BlendOpAlpha
	},	//	LIGHTEN
	{	//	DARKEN
		D3D11_BLEND_ONE,				//	SrcBlend
		D3D11_BLEND_ONE,				//	DestBlend
		D3D11_BLEND_OP_MIN,				//	BlendOp
		D3D11_BLEND_ONE,				//	SrcBlendAlpha
		D3D11_BLEND_ONE,				//	DestBlendAlpha
		D3D11_BLEND_OP_MIN				//	BlendOpAlpha
	},	//	DARKEN
	{	//	SCREEN
		D3D11_BLEND_SRC_ALPHA,			//	SrcBlend
		D3D11_BLEND_INV_SRC_COLOR,		//	DestBlend
		D3D11_BLEND_OP_ADD,				//	BlendOp
		D3D11_BLEND_ONE,				//	SrcBlendAlpha
		D3D11_BLEND_INV_SRC_ALPHA,		//	DestBlendAlpha
		D3D11_BLEND_OP_ADD				//	BlendOpAlpha
	}	//	SCREEN
};

void Blender::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	if (bLoad)	return;			
	HRESULT	hr = S_OK;
	for (int i = 0; i < MODE_MAX; i++)
	{
		_blend_data& _bd = blend_data[i];

		D3D11_BLEND_DESC bDesc       = {};
		bDesc.AlphaToCoverageEnable  = FALSE;
		bDesc.IndependentBlendEnable = FALSE;

		D3D11_RENDER_TARGET_BLEND_DESC &rtbd = bDesc.RenderTarget[0];
		rtbd.BlendEnable			= (i != NONE ? TRUE : FALSE);
		rtbd.SrcBlend				= _bd.SrcBlend;
		rtbd.DestBlend				= _bd.DestBlend;
		rtbd.BlendOp				= _bd.BlendOp;
		rtbd.SrcBlendAlpha			= _bd.SrcBlendAlpha;
		rtbd.DestBlendAlpha			= _bd.DestBlendAlpha;
		rtbd.BlendOpAlpha			= _bd.BlendOpAlpha;
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&bDesc, &blendState[i]);
		if (FAILED(hr)) assert(0 && "ブレンド作成失敗");
	}

	keepContext = context;
	nowMode = MODE_MAX;		//	異常値(未設定状態)
	bLoad = true;
}

void Blender::Set(BLEND_MODE mode, ID3D11DeviceContext* context)
{
	//	エラーチェック
	if (!bLoad)							return;
	if (mode < 0 || mode >= MODE_MAX)	return;
	if (mode == nowMode)				return;

	//	第2引数省略時、事前に設定しているcontextを使用
	if (!context)	context = keepContext;

	//	ブレンドの設定
	context->OMSetBlendState(blendState[mode], nullptr, 0xFFFFFFFF);
	nowMode = mode;
}

void Blender::Release()
{
	for (auto &p : blendState)
	{
		if (p) p->Release();
		p = nullptr;
	}

	keepContext = nullptr;
	nowMode = MODE_MAX;				//	異常値(未設定状態)
	bLoad = false;
}