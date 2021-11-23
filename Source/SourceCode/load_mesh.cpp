#include "load_mesh.h"

std::map<std::string, std::shared_ptr<ModelResource>, std::less<>> MeshManager::Data;

void MeshManager::Load()
{
    ModelData* robot = FbxLoader::GetInstance()->LoadFile("Data\\FBX\\Robot\\Robo_Anime_Run.fbx");
    FbxLoader::GetInstance()->AddAnimation("Data\\FBX\\Robot\\Robo_Anime_Sliding_start.fbx", *robot);
    FbxLoader::GetInstance()->AddAnimation("Data\\FBX\\Robot\\Robo_Anime_Sliding.fbx", *robot);
    FbxLoader::GetInstance()->AddAnimation("Data\\FBX\\Robot\\Robo_Anime_Jump_short.fbx", *robot);
    FbxLoader::GetInstance()->AddAnimation("Data\\FBX\\Robot\\Robo_Anime_Jump_mideum.fbx", *robot);
    FbxLoader::GetInstance()->AddAnimation("Data\\FBX\\Robot\\Robo_Anime_Jump_Large.fbx", *robot);
    FbxLoader::GetInstance()->AddAnimation("Data\\FBX\\Robot\\Robo_Anime_Jump_medeum_dash.fbx", *robot);
    Data["Robot"] = std::make_shared<ModelResource>(robot);

    Data["Stage"] = std::make_shared<ModelResource>(FbxLoader::GetInstance()->LoadFile("Data\\FBX\\Stage\\stage.fbx"));
    Data["Block01"] = std::make_shared<ModelResource>(FbxLoader::GetInstance()->LoadFile("Data\\FBX\\Stage\\block01.fbx"));
    Data["Block02"] = std::make_shared<ModelResource>(FbxLoader::GetInstance()->LoadFile("Data\\FBX\\Stage\\block02.fbx"));
    Data["Block03"] = std::make_shared<ModelResource>(FbxLoader::GetInstance()->LoadFile("Data\\FBX\\Stage\\block03.fbx"));
    Data["Block04"] = std::make_shared<ModelResource>(FbxLoader::GetInstance()->LoadFile("Data\\FBX\\Stage\\block04.fbx"));
    Data["Block05"] = std::make_shared<ModelResource>(FbxLoader::GetInstance()->LoadFile("Data\\FBX\\Stage\\block05.fbx"));

    Data["Cube"] = std::make_shared<ModelResource>(FbxLoader::GetInstance()->LoadFile("Data\\OBJ\\cube.obj"));
}

void MeshManager::Release()
{
    Data.clear();
}
