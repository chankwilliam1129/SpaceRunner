#pragma once
#include "../GameLib/sprite.h"
#include "../GameLib/texture.h"
#include "../GameLib/shader.h"
#include "component.h"
#include "transform.h"

class SpriteRender :public Component
{
public:
	enum PivotType
	{
		UpLeft,
		Up,
		UpRight,
		Center,
		BottomLeft,
		Bottom,
		BottomRight,
	};

public:
	Shader* pixelShader = nullptr;
	Shader* vertexShader = nullptr;
	Sprite* sprite = {};
	Texture* texture = {};
	Vector2 texturePosition = {};
	Vector2 drawTextureSize = { SCREEN_WIDTH,SCREEN_HEIGHT };
	Vector2 textureSize = { SCREEN_WIDTH,SCREEN_HEIGHT };
	Vector2 center = {};
	Vector4 color = { 1.0f,1.0f ,1.0f ,1.0f };

public:
	SpriteRender();
	void Setup(Vector2 drawTextureSize, Vector2 textureSize, PivotType pivotType = (UpLeft), Vector2 texturePosition = Vector2(.0f, .0f));

public:
	void Draw();
	void TextureDraw();
	void ImGuiUpdate();
};
