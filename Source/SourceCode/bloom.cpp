#include "bloom.h"
#include "stage.h"
#include "stage_block.h"

void BloomManager::Initialize()
{
	name = "OrthoWindowManager";
	Instance("SetBloom")->AddComponent<SetBloom>();
	Instance("BloomBlur")->AddComponent<BloomBlur>();
}

void SetBloom::Start()
{
	renderTexture = new RenderTexture(GetSystemManager->device.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	BloomManager::GetInstance()->setBloom = renderTexture;
}

void SetBloom::Update() 
{
	object->ChildList.clear();
	Object* circle = object->AddChild("circle", { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0 }, {}, { 2.0f,2.0f,2.0f });
	SpriteRender* tempSprite1 = circle->AddComponent<SpriteRender>();
	tempSprite1->texture->tData[0] = TextureManager::GetInstance()->tData["Circle"];
	tempSprite1->vertexShader = ShaderManager::GetInstance()->vsData["Sprite"];
	tempSprite1->pixelShader = ShaderManager::GetInstance()->psData["Sprite"];
	tempSprite1->drawTextureSize = Vector2(256, 256);
	tempSprite1->textureSize = Vector2(256, 256);
	tempSprite1->center = { 128,128 };
}

void BloomBlur::Start() 
{
	BloomManager::GetInstance()->upSampling = Bluring(0.25f, BloomManager::GetInstance()->setBloom->GetShaderResourceView());
}

void BlendTexture::Start()
{
	RenderTexture* blur1 = Blur(0.1f, object->GetComponent<SetBloom>()->renderTexture);
	RenderTexture* blur2 = Blur(0.1f, blur1);
	RenderTexture* blur3 = Blur(0.1f, blur2);
	RenderTexture* blur4 = Blur(0.1f, blur3);
	RenderTexture* blur5 = Blur(0.1f, blur4);
	RenderTexture* blur6 = Blur(0.1f, blur5);
	RenderTexture* blur7 = Blur(0.1f, blur6);
	
	Object* debug = object->AddChild("debug");
	for (int i = 0; i < 1; i++) 
	{
		debug->AddComponent<DebugSampling>()->spriteRender->texture->tData[0] = blur1->GetShaderResourceView();
		debug->AddComponent<DebugSampling>()->spriteRender->texture->tData[0] = blur2->GetShaderResourceView();
		debug->AddComponent<DebugSampling>()->spriteRender->texture->tData[0] = blur3->GetShaderResourceView();
		debug->AddComponent<DebugSampling>()->spriteRender->texture->tData[0] = blur4->GetShaderResourceView();
		debug->AddComponent<DebugSampling>()->spriteRender->texture->tData[0] = blur5->GetShaderResourceView();
		debug->AddComponent<DebugSampling>()->spriteRender->texture->tData[0] = blur6->GetShaderResourceView();
		debug->AddComponent<DebugSampling>()->spriteRender->texture->tData[0] = blur7->GetShaderResourceView();
	}
}

RenderTexture* BlendTexture::Blur(float scale, RenderTexture* renderTexture) 
{
	/*Object* temp1 = this->AddChild("Sampling1", {}, {}, { scale,scale,1.0f });
	temp1->AddComponent<Sampling>()->spriteRender->texture->tData[0] = renderTexture->GetShaderResourceView();

	Object* temp2 = this->AddChild("Blur1", {}, {}, { scale,scale,1.0f });
	temp2->AddComponent<HorizontalBlur>()->spriteRender->texture->tData[0] = temp1->GetComponent<Sampling>()->renderTexture->GetShaderResourceView();

	Object* temp3 = this->AddChild("Blur2", {}, {}, { scale,scale,1.0f });
	temp3->AddComponent<VerticalBlur>()->spriteRender->texture->tData[0] = temp2->GetComponent<HorizontalBlur>()->renderTexture->GetShaderResourceView();

	Object* temp4 = this->AddChild("Sampling2", {}, {}, { scale,scale,1.0f });
	temp4->AddComponent<Sampling>()->spriteRender->texture->tData[0] = temp3->GetComponent<VerticalBlur>()->renderTexture->GetShaderResourceView();

	return temp4->GetComponent<Sampling>()->renderTexture;*/

	Object* temp1 = object->AddChild("Sampling1", {}, {}, { scale,scale,1.0f });
	temp1->parent = nullptr;
	temp1->AddComponent<Sampling>()->spriteRender->texture->tData[0] = renderTexture->GetShaderResourceView();;

	Object* temp2 = object->AddChild("Blur1", {}, {}, { scale,scale,1.0f });
	temp2->parent = nullptr;
	temp2->AddComponent<HorizontalBlur>()->spriteRender->texture->tData[0] = temp1->GetComponent<Sampling>()->renderTexture->GetShaderResourceView();

	Object* temp3 = object->AddChild("Blur2", {}, {}, { scale,scale,1.0f });
	temp3->parent = nullptr;
	temp3->AddComponent<VerticalBlur>()->spriteRender->texture->tData[0] = temp2->GetComponent<HorizontalBlur>()->renderTexture->GetShaderResourceView();

	Object* temp4 = object->AddChild("Sampling2", {}, {}, { 1.0f ,1.0f ,1.0f });
	temp4->parent = nullptr;
	temp4->AddComponent<Sampling>()->spriteRender->texture->tData[0] = temp3->GetComponent<VerticalBlur>()->renderTexture->GetShaderResourceView();

	return temp4->GetComponent<Sampling>()->renderTexture;
}