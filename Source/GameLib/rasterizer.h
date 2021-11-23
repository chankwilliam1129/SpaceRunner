#pragma once
#include <d3d11.h>

class Rasterizer 
{
public:
	typedef enum 
	{
		DEFAULT = 0,
		SPRITE,
		WIREFLAME,
		SOLID,
		MODE_MAX
	}MODE;

public:
	static void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
	static void Set(Rasterizer::MODE mode);
	static void Release();

private:
	Rasterizer() {};
	static ID3D11RasterizerState* rasterizerState[MODE_MAX];
	static ID3D11DeviceContext* keepContext;
};