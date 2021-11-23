#pragma once
#include "scene.h"

class SceneGame :public Scene, public Singleton<SceneGame>
{
private:
	enum GameState
	{
		MAIN_START,
		MAIN_UPDATE,
		MENU_START,
		MENU_UPDATE,
		MENU_END,
		TOTAL,
	};

private:
	int mainTimer = 0;
	int menuTimer = 0;

public:
	void Initialize();
	void Update();
	void TextureRender();
	void Render();
	void Release();

private:
	void MainStart();
	void MainUpdate();
	void MenuStart();
	void MenuUpdate();
	void MenuEnd();

private:
	void(SceneGame::* GameStateUpdate[GameState::TOTAL])() =
	{
		&SceneGame::MainStart,
		&SceneGame::MainUpdate,
		&SceneGame::MenuStart,
		&SceneGame::MenuUpdate,
		&SceneGame::MenuEnd,
	};

private:
	bool isPaused = false;
};