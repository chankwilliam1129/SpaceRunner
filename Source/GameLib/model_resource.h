#pragma once
#include "model_data.h"

class ModelResource
{
public:
	ModelResource(ModelData* data);
	~ModelResource() {}

	struct Material
	{
		DirectX::XMFLOAT4 color = { 0.8f, 0.8f, 0.8f, 1.0f };
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
	};

	struct Subset
	{
		u_int		startIndex = 0;
		u_int		indexCount = 0;
		Material*   material   = nullptr;
	};

	struct Mesh
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer>	vertexBuffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer>	indexBuffer  = nullptr;
		std::vector<Subset>						subsets;

		int										nodeIndex = 0;
		std::vector<int>						nodeIndices;
		std::vector<DirectX::XMFLOAT4X4*>		inverseTransforms;
	};

	const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
	const std::vector<ModelData::Node>& GetNodes() const { return m_data->nodes; }
	const std::vector<ModelData::Animation>& GetAnimations() const { return m_data->animations; }

private:
	ModelData*	m_data;
	std::vector<Material>	    m_materials;
	std::vector<Mesh>		    m_meshes;
};
