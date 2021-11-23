#pragma once
#include <fbxsdk.h> 
#include "model_data.h"
#include "singleton.h"

class FbxLoader :public Singleton<FbxLoader>
{
public:
	// FBX�t�@�C�����烂�f���f�[�^�ǂݍ���
	bool Load(const char* filename, ModelData& data);

	ModelData* LoadFile(const char* filename);

	void AddAnimation(const char* fbxFilename, ModelData& data);

private:
	//FbxLoader() {};

	// ���f���f�[�^���\�z
	bool BuildModel(const char* dirname, FbxScene* fbxScene, ModelData& data);

	// �m�[�h�f�[�^���\�z
	void BuildNodes(FbxNode* fbxNode, ModelData& data, int parentNodeIndex);
	void BuildNode(FbxNode* fbxNode, ModelData& data, int parentNodeIndex);

	// ���b�V���f�[�^���\�z
	void BuildMeshes(FbxNode* fbxNode, ModelData& data);
	void BuildMesh(FbxNode* fbxNode, FbxMesh* fbxMesh, ModelData& data);

	// �}�e���A���f�[�^���\�z
	void BuildMaterials(const char* dirname, FbxScene* fbxScene, ModelData& data);
	void BuildMaterial(const char* dirname, FbxSurfaceMaterial* fbxSurfaceMaterial, ModelData& data);

	// �A�j���[�V�����f�[�^���\�z
	void BuildAnimations(FbxScene* fbxScene, ModelData& data);

	// �C���f�b�N�X�̌���
	int FindNodeIndex(ModelData& data, const char* name);
	int FindMaterialIndex(FbxScene* fbxScene, const FbxSurfaceMaterial* fbxSurfaceMaterial);
};
