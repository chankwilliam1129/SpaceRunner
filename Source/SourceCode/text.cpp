#include "text.h"

void TextRender::AddText(const wchar_t* text, Vector2 position)
{
	Text* t = new Text;
	t->text = text;
	object->GetComponent<Transform>()->position = Vector3(position.x, position.y, .0f);
	TextList.push_back(t);
}

void TextRender::Draw()
{
	if (TextList.empty()) { return; }

	Vector2 pos = object->GetComponent<Transform>()->GetWorldPosition2D();

	FontManager::GetInstance()->font->Begin(GetSystemManager->context.Get());
	for (auto& it : TextList)
	{
		FontManager::GetInstance()->font->Draw(it->text, Vector2(pos.x, pos.y), Vector4(1.0f, .0f, 1.0f, 1.0f));
	}
	FontManager::GetInstance()->font->End(GetSystemManager->context.Get());
}