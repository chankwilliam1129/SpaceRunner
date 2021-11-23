#include "game_object.h"

void GameObjectManager::Update()
{
#ifdef USE_IMGUI
	ImGui::Begin(name.c_str());
#endif	
	for (auto& it : ObjectList)
	{
		it->Update();
#ifdef USE_IMGUI
		it->ImGuiUpdate();
#endif
	}
#ifdef USE_IMGUI
	ImGui::End();
#endif	
}

void GameObjectManager::TextureRender() 
{
	for (auto& it : ObjectList)
	{
		it->TextureDraw();
	}
}

void GameObjectManager::Render()
{
	for (auto& it : ObjectList)
	{
		it->Draw();
	}
}

void GameObjectManager::Release()
{
	ObjectList.clear();
}

Object* GameObjectManager::Instance(std::string name, Vector3 pos, Vector3 rotation, Vector3 scale)
{
	Object* obj = new Object;
	obj->name = name;
	obj->isActive = true;
	obj->AddComponent<Transform>();
	obj->GetComponent<Transform>()->position = pos;
	obj->GetComponent<Transform>()->rotation = rotation;
	obj->GetComponent<Transform>()->scale = scale;
	ObjectList.push_back(obj);
	return obj;
}
