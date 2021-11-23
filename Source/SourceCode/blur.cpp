#include "../GameLib/shader.h"
#include "sprite_render.h"
#include "ortho_window.h"
#include "blur.h"
#include "glow.h"

ID3D11ShaderResourceView* Blur::Bluring(float scale, ID3D11ShaderResourceView* sampTexture)
{
	Object* temp1 = object->AddChild("Sampling1", {}, {}, { scale,scale,0.0f });
	temp1->AddComponent<Sampling>();
	temp1->GetComponent<SpriteRender>()->texture->tData[0] = sampTexture;

	Object* temp2 = object->AddChild("Blur1", {}, {}, { scale,scale,0.0f });
	temp2->AddComponent<HorizontalBlur>();
	temp2->GetComponent<SpriteRender>()->texture->tData[0] = temp1->GetComponent<Sampling>()->renderTexture->GetShaderResourceView();

	Object* temp3 = object->AddChild("Blur2", {}, {}, { scale,scale,0.0f });
	temp3->AddComponent<VerticalBlur>();
	temp3->GetComponent<SpriteRender>()->texture->tData[0] = temp2->GetComponent<HorizontalBlur>()->renderTexture->GetShaderResourceView();

	Object* temp4 = object->AddChild("Sampling2", {}, {}, { 1.0f,1.0f,0.0f });
	temp4->AddComponent<Sampling>();
	temp4->GetComponent<SpriteRender>()->texture->tData[0] = temp3->GetComponent<VerticalBlur>()->renderTexture->GetShaderResourceView();

	return temp4->GetComponent<Sampling>()->renderTexture->GetShaderResourceView();
}


void GlowBlur::Start()
{
	GlowManager::GetInstance()->upSampling = Bluring(0.5f, GlowManager::GetInstance()->glowTexture->GetShaderResourceView());
}

