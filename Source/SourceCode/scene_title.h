#pragma once
#include "scene.h"
#include "title_object.h"

class SceneTitle :public Scene, public Singleton<SceneTitle>
{
public:
	void Initialize();
	void Update();
	void Render();
	void Release();
};