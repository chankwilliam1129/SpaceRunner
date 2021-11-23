#pragma once
#include "model_resource.h"
#include "constant_buffer.h"

class Mesh 
{
private:
	struct CbMesh
	{
		DirectX::XMFLOAT4X4	world;
	};

	struct CbSubset
	{
		DirectX::XMFLOAT4	materialColor;
	};

public:
	std::shared_ptr<ConstantBuffer<CbMesh>> meshConstantBuffer;
	std::shared_ptr<ConstantBuffer<CbSubset>> subsetConstantBuffer;

public:
	static const D3D11_INPUT_ELEMENT_DESC* CreateInputElementDesc(size_t& numElements)
	{
		static const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		numElements = _countof(inputElementDesc);
		return inputElementDesc;
	}

	Mesh() {};
	Mesh(std::shared_ptr<ModelResource>& resource);
	~Mesh() {}

	struct Node
	{
		const char* name;
		Node* parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	localTransform;
		DirectX::XMFLOAT4X4	worldTransform;
	};

	// 行列計算
	void CalculateLocalTransform();
	void CalculateWorldTransform(const DirectX::XMMATRIX& worldTransform);

	//テクスチャー
	void LoadTexture(ID3D11Device* device, const wchar_t* fileName);
	void LoadTexture(ID3D11ShaderResourceView* shaderResourceView);

	const std::vector<Node>& GetNodes() const { return nodes; }
	std::vector<Node>& GetNodes() { return nodes; }
	const ModelResource* GetResource() const { return resource.get(); }

	//描画
	void Render(ID3D11DeviceContext* context);

private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				nodes;
};

class SkinMesh
{
private:
	static const int MaxBones = 128;
	struct CbMesh
	{
		DirectX::XMFLOAT4X4	boneTransforms[MaxBones];
	};

	struct CbSubset
	{
		DirectX::XMFLOAT4	materialColor;
	};

public:
	std::shared_ptr<ConstantBuffer<CbMesh>> meshConstantBuffer;
	std::shared_ptr<ConstantBuffer<CbSubset>> subsetConstantBuffer;

public:
	static const D3D11_INPUT_ELEMENT_DESC* CreateInputElementDesc(size_t& numElements)
	{
		static const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONES",    0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		numElements = _countof(inputElementDesc);
		return inputElementDesc;
	}

	SkinMesh(std::shared_ptr<ModelResource>& resource);
	~SkinMesh() {}

	struct Node
	{
		const char* name;
		Node* parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	localTransform;
		DirectX::XMFLOAT4X4	worldTransform;
	};

	// アニメーション
	bool IsPlayAnimation() const { return currentAnimation >= 0; }
	void PlayAnimation(int animationIndex, bool loop = false);
	void UpdateAnimation(float elapsedTime);

	// 行列計算
	void CalculateLocalTransform();
	void CalculateWorldTransform(const DirectX::XMMATRIX& worldTransform);

	//テクスチャー
	void LoadTexture(ID3D11Device* device, const wchar_t* fileName);
	void LoadTexture(ID3D11ShaderResourceView* shaderResourceView);

	const std::vector<Node>& GetNodes() const { return nodes; }
	std::vector<Node>& GetNodes() { return nodes; }
	const ModelResource* GetResource() const { return resource.get(); }

	//描画
	void Render(ID3D11DeviceContext* context);

private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				nodes;
	int								currentAnimation = -1;
	float							currentSeconds = 0.0f;
	bool							loopAnimation = false;
	bool							endAnimation = false;
};