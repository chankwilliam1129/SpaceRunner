#include "scene_tutorial.h"
#include "scene_title.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "stage.h"
#include "bloom.h"
#include "glow.h"

void SceneTutorial::Initialize()
{
	index = Tutorial;
	Scene::Initialize();
	CameraManager::GetInstance()->Initialize();
	CameraManager::GetInstance()->Instance("MainCamera", Vector3(0.0f, 0.0f, -50.0f), Vector3(0.0f, 0.0f, 0.0f))->AddComponent<Camera>();
	LightManager::GetInstance()->Initialize();
	GlowManager::GetInstance()->Initialize();
	StageManager::GetInstance()->Initialize();
	PlayerManager::GetInstance()->Initialize();
}

void SceneTutorial::Update()
{
	debug::SetString("%f", time);
	if (kb.PushTrigger(VK_RETURN))
	{
		SetScene(std::make_shared<SceneTitle>());
		return;
	}

	CameraManager::GetInstance()->Update();
	GlowManager::GetInstance()->Update();
	StageManager::GetInstance()->Update();
	PlayerManager::GetInstance()->Update();
	//BloomManager::GetInstance()->Update();
	LightManager::GetInstance()->sceneConstantBuffer->UpdateSubresource(GetSystemManager->context.Get());
}

void SceneTutorial::TextureRender()
{
	Blender::Set(Blender::ALPHA);
	Sampler::Set(Sampler::WRAP);
	Rasterizer::Set(Rasterizer::SOLID);
	StageManager::GetInstance()->TextureRender();
	PlayerManager::GetInstance()->TextureRender();
	GlowManager::GetInstance()->TextureRender();
}

void SceneTutorial::Render()
{
	Blender::Set(Blender::ALPHA);
	Sampler::Set(Sampler::WRAP);
	Rasterizer::Set(Rasterizer::SOLID);

	PlayerManager::GetInstance()->Render();
	StageManager::GetInstance()->Render();
	Blender::Set(Blender::NONE);
}

void SceneTutorial::Release()
{
	GlowManager::GetInstance()->Release();
	CameraManager::GetInstance()->Release();
	StageManager::GetInstance()->Release();
	PlayerManager::GetInstance()->Release();
}