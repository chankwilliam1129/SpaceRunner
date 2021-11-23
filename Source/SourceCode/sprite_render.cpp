#include "../GameLib/texture.h"
#include "sprite_render.h"
#include "glow.h"


SpriteRender::SpriteRender()
{
	sprite = new Sprite;
	texture = new Texture;
}
void SpriteRender::Setup(Vector2 drawTextureSize, Vector2 textureSize, PivotType pivot, Vector2 texturePosition)
{
	switch (pivot)
	{
	case PivotType::UpLeft:
		this->center = Vector2(.0f, .0f);
		break;

	case PivotType::Up:
		this->center.x = drawTextureSize.x / 2;
		this->center.y = .0f;
		break;

	case PivotType::UpRight:
		this->center.x = drawTextureSize.x;
		this->center.y = .0f;
		break;

	case PivotType::Center:
		this->center = drawTextureSize / 2;
		break;

	case PivotType::BottomLeft:
		this->center.x = .0f;
		this->center.y = drawTextureSize.y;
		break;

	case PivotType::Bottom:
		this->center.x = drawTextureSize.x / 2;
		this->center.y = drawTextureSize.y;
		break;

	case PivotType::BottomRight:
		this->center.x = drawTextureSize.x;
		this->center.y = drawTextureSize.y;
		break;
	}

	this->drawTextureSize = drawTextureSize;
	this->textureSize = textureSize;
	this->texturePosition = texturePosition;
}

void SpriteRender::Draw()
{
	DepthStencil::Set(DepthStencil::DEPTH_2D, DepthStencil::STENCIL_2D);

	texture->Activate();
	vertexShader->Activate(GetSystemManager->context.Get());
	pixelShader->Activate(GetSystemManager->context.Get());

	Vector2 pos = object->GetComponent<Transform>()->GetWorldPosition2D();
	Vector3 scale = object->GetComponent<Transform>()->GetWorldScale();
	Vector3 rotation = object->GetComponent<Transform>()->GetWorldRotation();

	sprite->Render(GetSystemManager->context.Get(),
		pos.x, pos.y,
		scale.x, scale.y,
		texturePosition.x, texturePosition.y,
		drawTextureSize.x, drawTextureSize.y,
		textureSize.x, textureSize.y,
		center.x, center.y,
		rotation.z,
		color
	);
}

void SpriteRender::TextureDraw()
{
	Glow* glow = object->GetComponent<Glow>();
	if (glow != nullptr)
	{
		DepthStencil::Set(DepthStencil::DEPTH_2D, DepthStencil::STENCIL_2D);

		glow->texture->Activate();
		glow->vertexShader->Activate(GetSystemManager->context.Get());
		glow->pixelShader->Activate(GetSystemManager->context.Get());

		Vector2 pos = object->GetComponent<Transform>()->GetWorldPosition2D();
		Vector3 scale = object->GetComponent<Transform>()->GetWorldScale();
		Vector3 rotation = object->GetComponent<Transform>()->GetWorldRotation();

		sprite->Render(GetSystemManager->context.Get(),
			pos.x, pos.y,
			scale.x, scale.y,
			texturePosition.x, texturePosition.y,
			drawTextureSize.x, drawTextureSize.y,
			textureSize.x, textureSize.y,
			center.x, center.y,
			rotation.z,
			color
		);
	}
}

void SpriteRender::ImGuiUpdate()
{
#ifdef USE_IMGUI
	if (ImGui::TreeNode("SpriteRender"))
	{
		ImGui::DragFloat2("texturePosition", &texturePosition.x, 0.1f);
		ImGui::DragFloat2("drawTextureSize", &drawTextureSize.x, 0.5f);
		ImGui::DragFloat2("textureSize", &textureSize.x, 0.1f);
		ImGui::TreePop();
	}
	ImGui::SetNextTreeNodeOpen(false, ImGuiSetCond_Once);
#endif
}