#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <sstream>
#include "../GameLib/system.h"
#include"../GameLib/singleton.h"
#include "../GameLib/constant_buffer.h"

using namespace DirectX;

struct SpotLight 
{
	float index;
	float range;//Œõ‚Ì“Í‚­”ÍˆÍ
	float type; //—LŒø‚©–³Œø‚©
	float innerCorn;
	float outerCorn;
	float dumy0;
	float dumy1;
	float dumy2;
	XMFLOAT4 pos;
	XMFLOAT4 color;
	XMFLOAT4 dir;
};

struct PointLight 
{
	float index;
	float range;//Œõ‚Ì“Í‚­”ÍˆÍ
	float type; //—LŒø‚©–³Œø‚©
	float dumy;
	XMFLOAT4 pos;
	XMFLOAT4 color;
};

class Light
{
private:

public:
	static const int PointMAX = 32;
	static const int SpotMAX = 32;

	static XMFLOAT4 lightDir;
	static XMFLOAT4 dirLightColor;
	static XMFLOAT4 ambient;
	static PointLight pointLight[PointMAX];
	static SpotLight spotLight[SpotMAX];

	static void Initialize();
	static void SetDirLight(XMFLOAT3 dir, XMFLOAT3 color);

	static void SetAmbient(XMFLOAT3 amb);

	static void SetPointLight(int index, XMFLOAT3 pos, XMFLOAT3 color, float range);

	static void SetSpotLight(int index, XMFLOAT3 pos, XMFLOAT3 color, XMFLOAT3 dir, float range, float inner, float outer);
};

class LightManager :public Singleton<LightManager> 
{
private:
	struct  CbScene
	{
		DirectX::XMFLOAT4	lightDirection;
	};

public:
	std::shared_ptr<Light> sceneLight;
	std::shared_ptr<ConstantBuffer<CbScene>> sceneConstantBuffer;

	void Initialize();
};