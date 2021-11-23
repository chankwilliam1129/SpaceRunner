#include "button.h"

void Button::Set(Type type)
{
	this->type = type;
}

bool Button::IsTouch()
{
	Vector2 mousePos = mouse.GetPosition();
	Vector2 pos = object->GetComponent<Transform>()->GetWorldPosition2D();
	Vector3 scale = object->GetComponent<Transform>()->GetWorldScale();
	Vector2 size = object->GetComponent<SpriteRender>()->drawTextureSize;
	Vector2 center = object->GetComponent<SpriteRender>()->center;

	pos += size / 2.0f - center;
	switch (type)
	{
	case Button::Rect:
	{
		Vector2 leftTop, rightBottom;
		leftTop.x = pos.x - center.x * scale.x;
		leftTop.y = pos.y - center.y * scale.y;
		rightBottom.x = pos.x + (size.x - center.x) * scale.x;
		rightBottom.y = pos.y + (size.y - center.y) * scale.y;

		if (mousePos.x < leftTop.x) { return false; }
		if (mousePos.x > rightBottom.x) { return false; }
		if (mousePos.y < leftTop.y) { return false; }
		if (mousePos.y > rightBottom.y) { return false; }
		return true;
		break;
	}
	case Button::Circle:
	{
		Vector2 vec = pos - mousePos;
		float distance = vec.x * vec.x + vec.y * vec.y;
		return (size.x * size.x > distance);
		break;
	}
	}
}

bool Button::IsClick()
{
	return (IsTouch() && mouse.ReleaseTrigger(Mouse::LEFT));
}

void Button::Update()
{
	if (IsClick())
	{
		debug::SetString("aaaa");
	}
}