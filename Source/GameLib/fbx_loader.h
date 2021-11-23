#pragma once
#include <fbxsdk.h> 
#include "model_data.h"
#include "singleton.h"

class FbxLoader :public Singleton<FbxLoader>
{
public:
	// FBXファイルからモデルデータ読み込み
	bool Load(const char* filename, ModelData& data);

	ModelData* LoadFile(const char* filename);

	void AddAnimation(const char* fbxFilename, ModelData& data);

private:
	//FbxLoader() {};

	// モデルデータを構築
	bool BuildModel(const char* dirname, FbxScene* fbxScene, ModelData& data);

	// ノードデータを構築
	void BuildNodes(FbxNode* fbxNode, ModelData& data, int parentNodeIndex);
	void BuildNode(FbxNode* fbxNode, ModelData& data, int parentNodeIndex);

	// メッシュデータを構築
	void BuildMeshes(FbxNode* fbxNode, ModelData& data);
	void BuildMesh(FbxNode* fbxNode, FbxMesh* fbxMesh, ModelData& data);

	// マテリアルデータを構築
	void BuildMaterials(const char* dirname, FbxScene* fbxScene, ModelData& data);
	void BuildMaterial(const char* dirname, FbxSurfaceMaterial* fbxSurfaceMaterial, ModelData& data);

	// アニメーションデータを構築
	void BuildAnimations(FbxScene* fbxScene, ModelData& data);

	// インデックスの検索
	int FindNodeIndex(ModelData& data, const char* name);
	int FindMaterialIndex(FbxScene* fbxScene, const FbxSurfaceMaterial* fbxSurfaceMaterial);
};
