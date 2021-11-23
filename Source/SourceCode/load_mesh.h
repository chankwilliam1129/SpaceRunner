#pragma once
#include <map>
#include <string>
#include "../GameLib/system.h"
#include "../GameLib/fbx_loader.h"
#include "../GameLib/mesh.h"
#include "../GameLib/singleton.h"

class MeshManager :public Singleton<MeshManager>
{
public:
	static std::map<std::string, std::shared_ptr<ModelResource>, std::less<>> Data;

public:
	static void Load();
	static void Release();
};