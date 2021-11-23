#include "../GameLib/shader.h"
#include "sprite_render.h"
#include "ortho_window.h"

void DebugSampling::Start() 
{
	spriteRender = object->AddComponent<SpriteRender>();
	spriteRender->vertexShader = ShaderManager::GetInstance()->vsData["Sprite"];
	spriteRender->pixelShader = ShaderManager::GetInstance()->psData["Sprite"];
	spriteRender->drawTextureSize = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
	spriteRender->textureSize = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Sampling::Start()
{
	renderTexture = new RenderTexture(GetSystemManager->device.Get(), SCREEN_WIDTH * object->GetComponent<Transform>()->scale.x, SCREEN_HEIGHT * object->GetComponent<Transform>()->scale.y);
	spriteRender = object->AddComponent<SpriteRender>();
	spriteRender->vertexShader = ShaderManager::GetInstance()->vsData["Sprite"];
	spriteRender->pixelShader = ShaderManager::GetInstance()->psData["Sprite"];
	spriteRender->drawTextureSize = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
	spriteRender->textureSize = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void HorizontalBlur::Start() 
{
	renderTexture = new RenderTexture(GetSystemManager->device.Get(), SCREEN_WIDTH * object->GetComponent<Transform>()->scale.x, SCREEN_HEIGHT * object->GetComponent<Transform>()->scale.y);

	cbObject = new ConstantBuffer<CbObject>(GetSystemManager->device.Get());
	cbObject->data.screenWidth = renderTexture->GetViewPort()->Width;
	cbObject->UpdateSubresource(GetSystemManager->context.Get());

	spriteRender = object->AddComponent<SpriteRender>();
	spriteRender->vertexShader = ShaderManager::GetInstance()->vsData["HorizontalBlur"];
	spriteRender->vertexShader->cbData[0] = cbObject;
	spriteRender->pixelShader = ShaderManager::GetInstance()->psData["HorizontalBlur"];
	spriteRender->drawTextureSize = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
	spriteRender->textureSize = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void VerticalBlur::Start()
{
	renderTexture = new RenderTexture(GetSystemManager->device.Get(), SCREEN_WIDTH * object->GetComponent<Transform>()->scale.x, SCREEN_HEIGHT * object->GetComponent<Transform>()->scale.y);

	cbObject = new ConstantBuffer<CbObject>(GetSystemManager->device.Get());
	cbObject->data.screenHeight = renderTexture->GetViewPort()->Height;
	cbObject->UpdateSubresource(GetSystemManager->context.Get());

	spriteRender = object->AddComponent<SpriteRender>();
	spriteRender->vertexShader = ShaderManager::GetInstance()->vsData["VeticalBlur"];
	spriteRender->vertexShader->cbData[0] = cbObject;
	spriteRender->pixelShader = ShaderManager::GetInstance()->psData["VeticalBlur"];
	spriteRender->drawTextureSize = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
	spriteRender->textureSize = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
}

GausianBlur::CbObject GausianBlur::CalcBlurParam(int width, int height, Vector2 dir, float deviation, float multiply)
{
	CbObject result;
	result.SampleCount = 15;
	auto tu = 1.0f / float(width);
	auto tv = 1.0f / float(height);

	result.Offset[0].z = GaussianDistribution(Vector2(0.0f, 0.0f), deviation) * multiply;
	auto total_weight = result.Offset[0].z;

	result.Offset[0].x = 0.0f;
	result.Offset[0].y = 0.0f;

	for (auto i = 1; i < 8; ++i)
	{
		result.Offset[i].x = dir.x * i * tu;
		result.Offset[i].y = dir.y * i * tv;
		result.Offset[i].z = GaussianDistribution(dir * float(i), deviation) * multiply;
		total_weight += result.Offset[i].z * 2.0f;
	}

	for (auto i = 0; i < 8; ++i)
	{
		result.Offset[i].z /= total_weight;
	}
	for (auto i = 8; i < 15; ++i)
	{
		result.Offset[i].x = -result.Offset[i - 7].x;
		result.Offset[i].y = -result.Offset[i - 7].y;
		result.Offset[i].z = result.Offset[i - 7].z;
	}

	return result;
}

void GausianHBlur::Start() 
{
	renderTexture = new RenderTexture(GetSystemManager->device.Get(), SCREEN_WIDTH * object->GetComponent<Transform>()->scale.x, SCREEN_HEIGHT * object->GetComponent<Transform>()->scale.y);

	cbObject = new ConstantBuffer<CbObject>(GetSystemManager->device.Get());
	cbObject->data = CalcBlurParam(renderTexture->GetViewPort()->Width, renderTexture->GetViewPort()->Height, Vector2(1, 0), 2.5f,2);
	cbObject->UpdateSubresource(GetSystemManager->context.Get());

	spriteRender = object->AddComponent<SpriteRender>();
	spriteRender->vertexShader = ShaderManager::GetInstance()->vsData["Sprite"];
	spriteRender->pixelShader = ShaderManager::GetInstance()->psData["GausianBlur"];
	spriteRender->pixelShader->cbData[0] = cbObject;
	spriteRender->drawTextureSize = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
	spriteRender->textureSize = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void GausianVBlur::Start() 
{
	renderTexture = new RenderTexture(GetSystemManager->device.Get(), SCREEN_WIDTH * object->GetComponent<Transform>()->scale.x, SCREEN_HEIGHT * object->GetComponent<Transform>()->scale.y);

	cbObject = new ConstantBuffer<CbObject>(GetSystemManager->device.Get());
	cbObject->data = CalcBlurParam(renderTexture->GetViewPort()->Width, renderTexture->GetViewPort()->Height, Vector2(0, 1), 2.5f,2);
	cbObject->UpdateSubresource(GetSystemManager->context.Get());

	spriteRender = object->AddComponent<SpriteRender>();
	spriteRender->vertexShader = ShaderManager::GetInstance()->vsData["Sprite"];
	spriteRender->pixelShader = ShaderManager::GetInstance()->psData["GausianBlur"];
	spriteRender->pixelShader->cbData[0] = cbObject;
	spriteRender->drawTextureSize = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
	spriteRender->textureSize = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
}