#pragma once
#include <DirectXMath.h>
#include "../GameLib/debug.h"
#include "../GameLib/font.h"
#include "../GameLib/input.h"
#include "../GameLib/sound.h"
#include "../GameLib/singleton.h"
#include "time.h"

class Scene
{
protected:
	std::shared_ptr<Scene> newScene = nullptr;

public:
	enum Index
	{
		Title,
		Game,
	};

public:
	int   index = 0;
	int   state = 0;
	double time = .0f;	

public:
	~Scene(){}
	virtual void Initialize()
	{
		state = 0;
		time = .0f;
		newScene = nullptr;
	};

	virtual void Update() {};
	virtual void TextureRender() {};
	virtual void Render() {};
	virtual void Release() {};

public:
	void SetScene(std::shared_ptr<Scene> scene) { newScene = scene; }
	std::shared_ptr<Scene> NewScene() { return newScene; }
};

class SceneManager :public Singleton<SceneManager>
{
public:
	std::shared_ptr<Scene> scene;

public:
	SceneManager() :scene(nullptr) {}
	~SceneManager() {}

	void Update();
	void Render();
	void ChangeScene(std::shared_ptr<Scene> newScene);
};
