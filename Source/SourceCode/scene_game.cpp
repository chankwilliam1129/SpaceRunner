#include "scene_game.h"
#include "scene_title.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "stage.h"
#include "bloom.h"
#include "glow.h"

void SceneGame::Initialize()
{
	index = Game;
	Scene::Initialize();
	CameraManager::GetInstance()->Initialize();
	CameraManager::GetInstance()->Instance("MainCamera", Vector3(0.0f, 0.0f, -50.0f), Vector3(0.0f, 0.0f, 0.0f))->AddComponent<Camera>();
	LightManager::GetInstance()->Initialize();
	GlowManager::GetInstance()->Initialize();
	StageManager::GetInstance()->Initialize();
	PlayerManager::GetInstance()->Initialize();
}

void SceneGame::Update()
{
	debug::SetString("%f", time);
	if (kb.PushTrigger(VK_RETURN))
	{
		SetScene(std::make_shared<SceneTitle>());
		return;
	}

	(this->*GameStateUpdate[state])();

	CameraManager::GetInstance()->Update();
	GlowManager::GetInstance()->Update();
	StageManager::GetInstance()->Update();
	PlayerManager::GetInstance()->Update();
	LightManager::GetInstance()->sceneConstantBuffer->UpdateSubresource(GetSystemManager->context.Get());
}

void SceneGame::TextureRender()
{
	Blender::Set(Blender::ALPHA);
	Sampler::Set(Sampler::WRAP);
	Rasterizer::Set(Rasterizer::SOLID);

	StageManager::GetInstance()->TextureRender();
	PlayerManager::GetInstance()->TextureRender();

	GlowManager::GetInstance()->TextureRender();
}

void SceneGame::Render()
{
	Blender::Set(Blender::ALPHA);
	Sampler::Set(Sampler::WRAP);
	Rasterizer::Set(Rasterizer::SOLID);

	StageManager::GetInstance()->Render();
	PlayerManager::GetInstance()->Render();
	Blender::Set(Blender::NONE);
}

void SceneGame::Release()
{
	GlowManager::GetInstance()->Release();
	CameraManager::GetInstance()->Release();
	StageManager::GetInstance()->Release();
	PlayerManager::GetInstance()->Release();
}

void SceneGame::MainStart()
{
	mainTimer = 0;
	state = GameState::MAIN_UPDATE;
}

void SceneGame::MainUpdate()
{

}

void SceneGame::MenuStart()
{
	menuTimer = 0;
	state = GameState::MENU_UPDATE;
}

void SceneGame::MenuUpdate()
{
}

void SceneGame::MenuEnd()
{
	state = GameState::MAIN_UPDATE;
}
