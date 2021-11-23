#pragma once
#include "game_object.h"

class GlowManager : public GameObjectManager , public Singleton<GlowManager>
{
public:
	struct CbGlow
	{
		float glowStrength;
		int padding[3];
	};
	ConstantBuffer<CbGlow>* cbGlow;

public:
	RenderTexture* glowTexture;
	ID3D11ShaderResourceView* upSampling;

public:
	void Initialize();
	void Update();
	void TextureRender();
	void ImguiUpdate();
};


class Glow :public Component {
public:
	RenderTexture* renderTexture;
	Shader* pixelShader = nullptr;
	Shader* vertexShader = nullptr;
	Texture* texture = nullptr;

public:
	Glow() { texture = new Texture; };
public:
	void Start();
};