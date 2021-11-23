#pragma once
#include "../GameLib/render_texture.h"
#include "../GameLib/constant_buffer.h"
#include "component.h"

class Blur :public Component
{
public:
	ID3D11ShaderResourceView* Bluring(float scale, ID3D11ShaderResourceView* sampTexture);
};

class GlowBlur :public Blur
{
public:
	void Start();
};