#pragma once
#include <d3d11.h>

class DepthStencil 
{
public:
	typedef enum 
	{
		DEPTH_3D,
		DEPTH_2D,
		DEPTH_MAX
	}DEPTH_MODE;

	typedef enum 
	{
		STENCIL_3D = 0,
		STENCIL_2D,
		MASK_INCR,
		MASK_DECR,
		MASK_APPLY,
		STENCIL_MAX
	}STENCIL_MODE;

public:
	static void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
	static void Set(DEPTH_MODE, STENCIL_MODE);
	static void Release();

private:
	static ID3D11DepthStencilState* depthStencilState[DEPTH_MODE::DEPTH_MAX][STENCIL_MODE::STENCIL_MAX];
	static ID3D11DeviceContext* keepContext;
	static DEPTH_MODE nowDepthMode;
	static STENCIL_MODE nowStencilMode;

	DepthStencil();
};