#pragma once
#include <string>
#include "../GameLib/system.h"
#include "../GameLib/font.h"
#include "component.h"
#include "transform.h"

class Text
{
public:
	const wchar_t* text = nullptr;
};

class TextRender :public Component
{
public:
	std::list<Text*> TextList;

public:
	TextRender() {};
	void AddText(const wchar_t* text, Vector2 position = { 0,0 });
	void Draw();
};
