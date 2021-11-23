#include "scene_title.h"
#include "scene_game.h"

void SceneTitle::Initialize()
{
	index = Title;
	Scene::Initialize();
	CameraManager::GetInstance()->Initialize();
	CameraManager::GetInstance()->Instance("MainCamera", Vector3(-5.0f, 9.0f, -12.0f), Vector3(10.0f, 32.0f, .0f))->AddComponent<Camera>();
	LightManager::GetInstance()->Initialize();
	TitleManager::GetInstance()->Initialize();
	SoundManager::GetInstance()->Data["BGM"]->Play(true);
	SoundManager::GetInstance()->Data["BGM"]->SetVolume(0.7f);
}

void SceneTitle::Update()
{
	debug::SetString("%f", time);
	switch (state)
	{
	case 0:
		if (kb.PushTrigger(VK_SPACE))
		{
			SoundManager::GetInstance()->Data["Decision"]->Play(false);
			state++;
		}
		break;
	case 1:
		if (kb.PushTrigger(VK_SPACE))
		{
			SoundManager::GetInstance()->Data["Decision"]->Play(false);
			state++;
		}
		break;
	case 2:
		if (kb.PushTrigger(VK_SPACE))
		{
			SoundManager::GetInstance()->Data["Decision"]->Play(false);
			SetScene(std::make_shared<SceneGame>());
			return;
		}
		break;
	}
	CameraManager::GetInstance()->Update();
	TitleManager::GetInstance()->Update();
	LightManager::GetInstance()->sceneConstantBuffer->UpdateSubresource(GetSystemManager->context.Get());
}

void SceneTitle::Render()
{
	Blender::Set(Blender::ALPHA);
	Sampler::Set(Sampler::WRAP);
	Rasterizer::Set(Rasterizer::SOLID);

	TitleManager::GetInstance()->Render();
}

void SceneTitle::Release()
{
	CameraManager::GetInstance()->Release();
	TitleManager::GetInstance()->Release();
}