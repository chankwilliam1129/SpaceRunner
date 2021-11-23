#pragma once
#include <d3d11.h>

class Sampler 
{
public:
	typedef enum 
	{
		NONE = 0,	
		SPRITE,
		WRAP,
		Clamp,
		MODE_MAX
	}MODE;

	static void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
	static void Set(Sampler::MODE mode);
	static void Release();

private:
	static ID3D11SamplerState* samplerState[MODE_MAX];
	static ID3D11DeviceContext* keepContext;

	Sampler();
};