#include "model_resource.h"
#include "resource_manager.h"
#include "misc.h"
#include "system.h"

ModelResource::ModelResource(ModelData* data)
{
	ID3D11Device* device = GetSystemManager->device.Get();

	m_data = std::move(data);

	// マテリアル
	m_materials.resize(m_data->materials.size());
	for (size_t material_index = 0; material_index < m_materials.size(); ++material_index)
	{
		auto&& src = m_data->materials.at(material_index);
		auto&& dst = m_materials.at(material_index);

		dst.color = src.color;

		// テクスチャ読み込み
		if (!src.textureFilename.empty())
		{
			size_t length;
			wchar_t filename[256];
			::mbstowcs_s(&length, filename, 256, src.textureFilename.c_str(), _TRUNCATE);

			D3D11_TEXTURE2D_DESC texture2DDesc = {};
			ResourceManager::CreateShaderResourceView(device, filename, dst.shaderResourceView.GetAddressOf(), texture2DDesc);
		}
	}

	// メッシュ
	m_meshes.resize(m_data->meshes.size());
	for (size_t mesh_index = 0; mesh_index < m_meshes.size(); ++mesh_index)
	{
		auto&& src = m_data->meshes.at(mesh_index);
		auto&& dst = m_meshes.at(mesh_index);

		// 頂点バッファ
		{
			D3D11_BUFFER_DESC buffer_desc = {};
			D3D11_SUBRESOURCE_DATA subresource_data = {};

			buffer_desc.ByteWidth = static_cast<UINT>(sizeof(ModelData::Vertex) * src.vertices.size());
			//buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
			buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			buffer_desc.CPUAccessFlags = 0;
			buffer_desc.MiscFlags = 0;
			buffer_desc.StructureByteStride = 0;
			subresource_data.pSysMem = src.vertices.data();
			subresource_data.SysMemPitch = 0;
			subresource_data.SysMemSlicePitch = 0;

			HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, dst.vertexBuffer.GetAddressOf());
			//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}

		// インデックスバッファ
		{
			D3D11_BUFFER_DESC buffer_desc = {};
			D3D11_SUBRESOURCE_DATA subresource_data = {};

			buffer_desc.ByteWidth = static_cast<UINT>(sizeof(u_int) * src.indices.size());
			//buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
			buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			buffer_desc.CPUAccessFlags = 0;
			buffer_desc.MiscFlags = 0;
			buffer_desc.StructureByteStride = 0;
			subresource_data.pSysMem = src.indices.data();
			subresource_data.SysMemPitch = 0; //Not use for index buffers.
			subresource_data.SysMemSlicePitch = 0; //Not use for index buffers.
			HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, dst.indexBuffer.GetAddressOf());
			//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}

		dst.nodeIndex = src.nodeIndex;

		// サブセット
		dst.subsets.resize(src.subsets.size());
		for (size_t subset_index = 0; subset_index < src.subsets.size(); ++subset_index)
		{
			auto&& src_subset = src.subsets.at(subset_index);
			auto&& dst_subset = dst.subsets.at(subset_index);

			dst_subset.startIndex = src_subset.startIndex;
			dst_subset.indexCount = src_subset.indexCount;
			dst_subset.material = &m_materials.at(src_subset.materialIndex);
		}

		// ボーン変換行列用
		dst.nodeIndices.resize(src.nodeIndices.size());
		::memcpy(dst.nodeIndices.data(), src.nodeIndices.data(), sizeof(int) * dst.nodeIndices.size());

		dst.inverseTransforms.resize(src.inverseTransforms.size());
		for (size_t index = 0; index < dst.inverseTransforms.size(); ++index)
		{
			dst.inverseTransforms.at(index) = &src.inverseTransforms.at(index);
		}
	}
}
