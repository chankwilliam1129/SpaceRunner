#pragma once
#include "game_object.h"
#include "blur.h"

class BloomManager :public GameObjectManager, public Singleton<BloomManager>
{
public:
	RenderTexture* setBloom;
	ID3D11ShaderResourceView* upSampling;
public:
	void Initialize();
};

class SetBloom :public OrthoWindow
{
public:
	void Start();
	void Update();
};

class BloomBlur :public Blur{
public:
	void Start();
};

class BlendTexture :public Component 
{
private:
	RenderTexture* Blur(float scale, RenderTexture* renderTexture);
public:
	void Start();
};