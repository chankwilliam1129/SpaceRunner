#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include <string>

struct ModelData
{
	struct Node
	{
		std::string			name;
		int					parentIndex;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
	};

	struct Vertex
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT3	normal;
		DirectX::XMFLOAT2	texcoord;
		DirectX::XMFLOAT4	bone_weight;
		DirectX::XMUINT4	bone_index;
	};

	struct Subset
	{
		int					materialIndex;
		int					startIndex;
		int					indexCount;
	};

	struct Mesh
	{
		std::vector<Vertex>	vertices;
		std::vector<int>	indices;
		std::vector<Subset>	subsets;

		int					nodeIndex;

		std::vector<int>					nodeIndices;
		std::vector<DirectX::XMFLOAT4X4>	inverseTransforms;
	};

	struct Material
	{
		DirectX::XMFLOAT4	color;
		std::string			textureFilename;
	};

	struct NodeKeyData
	{
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
	};

	struct Keyframe
	{
		float						seconds;
		std::vector<NodeKeyData>	nodeKeys;
	};
	struct Animation
	{
		float						secondsLength;
		std::vector<Keyframe>		keyframes;
	};

	std::vector<Node>		nodes;

	std::vector<Mesh>		meshes;
	std::vector<Material>	materials;

	std::vector<Animation>	animations;
};