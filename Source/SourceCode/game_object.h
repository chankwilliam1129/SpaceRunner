#pragma once
#include "component_list.h"
#include "load_mesh.h"
#include "time.h"

class GameObjectManager
{
public:
	std::string name;
    std::list<Object*> ObjectList;

public:
	void Initialize() {};
	void Update();
	void TextureRender();
	void Render();
	void Release();

public:
	Object* Instance(std::string name, Vector3 pos = Vector3(.0f, .0f, .0f), Vector3 rotation = Vector3(.0f, .0f, .0f), Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));
};