#pragma once
#include	<d3d11.h>

class Blender
{
public:
	typedef enum 
	{
		NONE = 0,	//	‡¬–³‚µ
		ALPHA,		//	ƒ¿‡¬
		ADD,		//	‰ÁZ‡¬
		SUBTRACT,	//	Œ¸Z‡¬
		REPLACE,	//	’u‚«Š·‚¦
		MULTIPLY,	//	æZ
		LIGHTEN,	//	”äŠr(–¾)
		DARKEN,		//	”äŠr(ˆÃ)
		SCREEN,		//	ƒXƒNƒŠ[ƒ“
		MODE_MAX
	}BLEND_MODE;

private:
	static ID3D11BlendState*    blendState[MODE_MAX];
	static bool					bLoad;
	static BLEND_MODE			nowMode;
	static ID3D11DeviceContext* keepContext;

public:
	Blender() {};			//	À‘ÌŠm•Û‹Ö~

public:
	static void	Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
	static void Set(BLEND_MODE mode, ID3D11DeviceContext* context = nullptr);
	static void Release();
};

