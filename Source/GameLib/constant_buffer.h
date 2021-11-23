#pragma once
#include <d3d11.h>
#include <wrl.h>

class Cbuffer
{
public:
	virtual void Activate(ID3D11DeviceContext* context, UINT slot, bool vs_on = true, bool ps_on = true) {};
	virtual void Set(ID3D11DeviceContext* context, UINT slot, bool vs_on = true, bool ps_on = true) {};
};

template <class T>
struct ConstantBuffer : public Cbuffer
{
	T data{};
	ConstantBuffer(ID3D11Device* device)
	{
		_ASSERT_EXPR(sizeof(T) % 16 == 0, L"constant buffer's need to be 16 byte aligned");
		HRESULT hr = S_OK;
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = sizeof(T);
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
#ifdef USAGE_DYNAMIC
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		hr = device->CreateBuffer(&buffer_desc, 0, buffer_object.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
#else
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA subresource_data;
		subresource_data.pSysMem = &data;
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, bufferObject.GetAddressOf());
#endif
	}

	virtual ~ConstantBuffer() {};

	void Activate(ID3D11DeviceContext* context, UINT slot, bool vs_on = true, bool ps_on = true)
	{
		HRESULT hr = S_OK;
#ifdef USAGE_DYNAMIC
		D3D11_MAP map = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mapped_buffer;

		hr = immediate_context->Map(buffer_object.Get(), 0, map, 0, &mapped_buffer);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		memcpy_s(mapped_buffer.pData, sizeof(cBuffer), &data, sizeof(cBuffer));
		immediate_context->Unmap(buffer_object.Get(), 0);
#else
		context->UpdateSubresource(bufferObject.Get(), 0, 0, &data, 0, 0);
#endif
		if (vs_on)
		{
			context->VSSetConstantBuffers(slot, 1u, bufferObject.GetAddressOf());
		}
		if (ps_on)
		{
			context->PSSetConstantBuffers(slot, 1u, bufferObject.GetAddressOf());
		}
	}

	void Set(ID3D11DeviceContext* context, UINT slot, bool vs_on = true, bool ps_on = true)
	{
		if (vs_on)
		{
			context->VSSetConstantBuffers(slot, 1u, bufferObject.GetAddressOf());
		}
		if (ps_on)
		{
			context->PSSetConstantBuffers(slot, 1u, bufferObject.GetAddressOf());
		}
	}

	void UpdateSubresource(ID3D11DeviceContext* context)
	{
		context->UpdateSubresource(bufferObject.Get(), 0, 0, &data, 0, 0);
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> bufferObject;
};