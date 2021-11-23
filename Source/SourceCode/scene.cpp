#include "../GameLib/system.h"
#include "scene.h"

void SceneManager::Update()
{
#ifdef USE_IMGUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif 

	std::shared_ptr<Scene> newScene = nullptr;
	newScene = scene->NewScene();
	if (newScene) 
	{		
		scene->Release();
		ChangeScene(std::move(newScene));
	}

	//キーボードの状態取得
	KeyBoard::GetState();

	scene->Update();

	scene->time += GetSystemManager->hrTimer.DeltaTime() * GetTimeManager->scale;
}

void SceneManager::Render()
{
	WindowManager::GetInstance()->ClearRenderTarget(Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	for (int i = 0; i < ViewPortManager::GetInstance()->screenNum; i++)
	{
		ViewPortManager::GetInstance()->SetViewPorts(1, &ViewPortManager::GetInstance()->viewPorts[i]);
		scene->TextureRender();
		scene->Render();
	}

#ifdef USE_IMGUI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

	//debug::Display(.0f, .0f, .0f, 1.2f, 1.2f);

	GetSystemManager->SetPresent(0, 0);
}

void SceneManager::ChangeScene(std::shared_ptr<Scene> newScene)
{
	scene = newScene;
	scene->Initialize();
}
