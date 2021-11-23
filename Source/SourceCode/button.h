#pragma once
#include "../GameLib/input.h"
#include "component.h"
#include "transform.h"
#include "sprite_render.h"

class Button :public Component
{
public:
	enum Type
	{
		Rect,
		Circle,
	};

	Type type = Rect;

public:
	Button() {};
	void Set(Type type);
	bool IsTouch();
	bool IsClick();
	void Update();
};