#include <vector>
#include <string>
#include <fstream>
#include <fbxsdk.h>
#include <functional>
#include "mesh.h"
#include "system.h"
#include "texture.h"
#include "resource_manager.h"
#include "misc.h"

using namespace fbxsdk;

Mesh::Mesh(std::shared_ptr<ModelResource>& resource)
{
	this->resource = resource;

	// ノード
	const std::vector<ModelData::Node>& res_nodes = resource->GetNodes();

	nodes.resize(res_nodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = res_nodes.at(nodeIndex);
		auto&& dst = nodes.at(nodeIndex);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;
	}

	meshConstantBuffer = std::make_shared<ConstantBuffer<CbMesh>>(GetSystemManager->device.Get());
	subsetConstantBuffer = std::make_shared<ConstantBuffer<CbSubset>>(GetSystemManager->device.Get());
	subsetConstantBuffer->data.materialColor = Vector4(1, 1, 1, 1);
}

// ローカル変換行列計算
void Mesh::CalculateLocalTransform()
{
	for (Node& node : nodes)
	{
		DirectX::XMMATRIX scale, rotate, translate;
		scale = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		rotate = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(node.rotate.x, node.rotate.y, node.rotate.z, node.rotate.w));
		translate = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMStoreFloat4x4(&node.localTransform, scale * rotate * translate);
	}
}

// ワールド変換行列計算
void Mesh::CalculateWorldTransform(const DirectX::XMMATRIX& worldTransform)
{
	for (Node& node : nodes)
	{
		DirectX::XMMATRIX localTransform = DirectX::XMLoadFloat4x4(&node.localTransform);
		if (node.parent != nullptr)
		{
			DirectX::XMMATRIX parentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
			DirectX::XMStoreFloat4x4(&node.worldTransform, localTransform * parentTransform);
		}
		else
		{
			DirectX::XMStoreFloat4x4(&node.worldTransform, localTransform * worldTransform);
		}
	}
}

void Mesh::LoadTexture(ID3D11Device* device, const wchar_t* fileName)
{
	D3D11_TEXTURE2D_DESC texture2D_desc = {};
	ResourceManager::CreateShaderResourceView(device, fileName, resource->GetMeshes().begin()->subsets.begin()->material->shaderResourceView.GetAddressOf(), texture2D_desc);
}

void Mesh::LoadTexture(ID3D11ShaderResourceView* shaderResourceView)
{
	resource->GetMeshes().begin()->subsets.begin()->material->shaderResourceView.Swap(shaderResourceView);
}

void Mesh::Render(ID3D11DeviceContext* context)
{
	const ModelResource* resource = GetResource();
	const std::vector<Mesh::Node>& nodes = GetNodes();

	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		if (mesh.nodeIndices.size() > 0)
		{
			for (size_t i = 0; i < mesh.nodeIndices.size(); ++i)
			{
				DirectX::XMMATRIX world_transform = DirectX::XMLoadFloat4x4(&nodes.at(mesh.nodeIndices.at(i)).worldTransform);
				DirectX::XMMATRIX inverse_transform = DirectX::XMLoadFloat4x4(mesh.inverseTransforms.at(i));
				DirectX::XMMATRIX bone_transform = inverse_transform * world_transform;
				DirectX::XMStoreFloat4x4(&meshConstantBuffer->data.world, bone_transform);
			}
		}
		else
		{
			meshConstantBuffer->data.world = nodes.at(mesh.nodeIndex).worldTransform;
		}
		meshConstantBuffer->Activate(context, 2, true, false);
		//meshConstantBuffer->UpdateSubresource(context);

		UINT stride = sizeof(ModelData::Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, mesh.vertexBuffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			//subsetConstantBuffer->data.materialColor = subset.material->color;
			subsetConstantBuffer->Activate(context, 3, true, false);
			//subsetConstantBuffer->UpdateSubresource(context);
			context->PSSetShaderResources(0, 1, subset.material->shaderResourceView.Get() ? subset.material->shaderResourceView.GetAddressOf() : DummyTexture::GetInstance()->GetTexture());
			context->DrawIndexed(subset.indexCount, subset.startIndex, 0);
		}
	}
}


SkinMesh::SkinMesh(std::shared_ptr<ModelResource>& resource)
{
	this->resource = resource;

	// ノード
	const std::vector<ModelData::Node>& res_nodes = resource->GetNodes();

	nodes.resize(res_nodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = res_nodes.at(nodeIndex);
		auto&& dst = nodes.at(nodeIndex);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;
	}

	meshConstantBuffer = std::make_shared<ConstantBuffer<CbMesh>>(GetSystemManager->device.Get());
	subsetConstantBuffer = std::make_shared<ConstantBuffer<CbSubset>>(GetSystemManager->device.Get());
	subsetConstantBuffer->data.materialColor = Vector4(1, 1, 1, 1);
}

// アニメーション再生
void SkinMesh::PlayAnimation(int animationIndex, bool loop)
{
	currentAnimation = animationIndex;
	loopAnimation = loop;
	endAnimation = false;
	currentSeconds = 0.0f;
}

// アニメーション計算
void SkinMesh::UpdateAnimation(float elapsedTime)
{
	if (currentAnimation < 0)
	{
		return;
	}

	if (resource->GetAnimations().empty())
	{
		return;
	}

	const ModelData::Animation& animation = resource->GetAnimations().at(currentAnimation);

	const std::vector<ModelData::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		// 現在の時間がどのキーフレームの間にいるか判定する
		const ModelData::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelData::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		if (currentSeconds >= keyframe0.seconds && currentSeconds < keyframe1.seconds)
		{
			float rate = (currentSeconds - keyframe0.seconds / keyframe1.seconds - keyframe0.seconds);

			assert(nodes.size() == keyframe0.nodeKeys.size());
			assert(nodes.size() == keyframe1.nodeKeys.size());
			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				// ２つのキーフレーム間の補完計算
				const ModelData::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelData::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				DirectX::XMVECTOR s0 = DirectX::XMLoadFloat3(&key0.scale);
				DirectX::XMVECTOR s1 = DirectX::XMLoadFloat3(&key1.scale);
				DirectX::XMVECTOR r0 = DirectX::XMLoadFloat4(&key0.rotate);
				DirectX::XMVECTOR r1 = DirectX::XMLoadFloat4(&key1.rotate);
				DirectX::XMVECTOR t0 = DirectX::XMLoadFloat3(&key0.translate);
				DirectX::XMVECTOR t1 = DirectX::XMLoadFloat3(&key1.translate);

				DirectX::XMVECTOR s = DirectX::XMVectorLerp(s0, s1, rate);
				DirectX::XMVECTOR r = DirectX::XMQuaternionSlerp(r0, r1, rate);
				DirectX::XMVECTOR t = DirectX::XMVectorLerp(t0, t1, rate);

				DirectX::XMStoreFloat3(&node.scale, s);
				DirectX::XMStoreFloat4(&node.rotate, r);
				DirectX::XMStoreFloat3(&node.translate, t);
			}
			break;
		}
	}

	// 最終フレーム処理
	if (endAnimation)
	{
		endAnimation = false;
		currentAnimation = -1;
		return;
	}

	// 時間経過
	currentSeconds += elapsedTime;
	if (currentSeconds >= animation.secondsLength)
	{
		if (loopAnimation)
		{
			currentSeconds -= animation.secondsLength;
		}
		else
		{
			currentSeconds = animation.secondsLength;
			endAnimation = true;
		}
	}
}

// ローカル変換行列計算
void SkinMesh::CalculateLocalTransform()
{
	for (Node& node : nodes)
	{
		DirectX::XMMATRIX scale, rotate, translate;
		scale = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		rotate = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(node.rotate.x, node.rotate.y, node.rotate.z, node.rotate.w));
		translate = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);

		DirectX::XMStoreFloat4x4(&node.localTransform, scale * rotate * translate);
	}
}

// ワールド変換行列計算
void SkinMesh::CalculateWorldTransform(const DirectX::XMMATRIX& worldTransform)
{
	for (Node& node : nodes)
	{
		DirectX::XMMATRIX localTransform = DirectX::XMLoadFloat4x4(&node.localTransform);
		if (node.parent != nullptr)
		{
			DirectX::XMMATRIX parentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
			DirectX::XMStoreFloat4x4(&node.worldTransform, localTransform * parentTransform);
		}
		else
		{
			DirectX::XMStoreFloat4x4(&node.worldTransform, localTransform * worldTransform);
		}
	}
}

void SkinMesh::LoadTexture(ID3D11Device* device, const wchar_t* fileName)
{
	D3D11_TEXTURE2D_DESC texture2D_desc = {};
	ResourceManager::CreateShaderResourceView(device, fileName, resource->GetMeshes().begin()->subsets.begin()->material->shaderResourceView.GetAddressOf(), texture2D_desc);
}

void SkinMesh::LoadTexture(ID3D11ShaderResourceView* shaderResourceView)
{
	resource->GetMeshes().begin()->subsets.begin()->material->shaderResourceView.Swap(shaderResourceView);
}

void SkinMesh::Render(ID3D11DeviceContext* context)
{
	const ModelResource* resource = GetResource();
	const std::vector<SkinMesh::Node>& nodes = GetNodes();

	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		if (mesh.nodeIndices.size() > 0)
		{
			for (size_t i = 0; i < mesh.nodeIndices.size(); ++i)
			{
				DirectX::XMMATRIX world_transform = DirectX::XMLoadFloat4x4(&nodes.at(mesh.nodeIndices.at(i)).worldTransform);
				DirectX::XMMATRIX inverse_transform = DirectX::XMLoadFloat4x4(mesh.inverseTransforms.at(i));
				DirectX::XMMATRIX bone_transform = inverse_transform * world_transform;
				DirectX::XMStoreFloat4x4(&meshConstantBuffer->data.boneTransforms[i], bone_transform);
			}
		}
		else
		{
			meshConstantBuffer->data.boneTransforms[0] = nodes.at(mesh.nodeIndex).worldTransform;
		}
		meshConstantBuffer->Activate(context, 2, true, false);
		//meshConstantBuffer->UpdateSubresource(context);

		UINT stride = sizeof(ModelData::Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, mesh.vertexBuffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			//subsetConstantBuffer->data.materialColor = subset.material->color;
			subsetConstantBuffer->Activate(context, 3, true, false);
			//subsetConstantBuffer->UpdateSubresource(context);
			context->PSSetShaderResources(0, 1, subset.material->shaderResourceView.Get() ? subset.material->shaderResourceView.GetAddressOf() : DummyTexture::GetInstance()->GetTexture());
			context->DrawIndexed(subset.indexCount, subset.startIndex, 0);
		}
	}
}