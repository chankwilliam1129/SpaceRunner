#include "texture.h"
#include "system.h"


void TextureManager::Create()
{
	tData["Number"] = Load(L"./Data/Sprites/number.png");
	tData["Title"] = Load(L"./Data/Sprites/title.png");
}

void Texture::Load(const wchar_t* file_name)
{
	ResourceManager::CreateShaderResourceView(GetSystemManager->device.Get(), file_name, &shaderResourceView, texture2dDesc);
}

void Texture::Load(ID3D11ShaderResourceView* t_shaderResourceView)
{
	shaderResourceView.Swap(t_shaderResourceView);
}

void Texture::Set(UINT Slot, BOOL flg)
{
	ID3D11DeviceContext* context = GetSystemManager->context.Get();

	if (flg == FALSE)
	{
		ID3D11ShaderResourceView* rtv[1] = { NULL };
		ID3D11SamplerState* ss[1] = { NULL };
		context->PSSetShaderResources(Slot, 1, rtv);
		context->PSSetSamplers(Slot, 1, ss);
		context->DSSetShaderResources(Slot, 1, rtv);
		context->DSSetSamplers(Slot, 1, ss);
		return;
	}
	if (shaderResourceView)
	{
		context->PSSetShaderResources(Slot, 1, shaderResourceView.GetAddressOf());
		context->DSSetShaderResources(Slot, 1, shaderResourceView.GetAddressOf());
	}
}

void Texture::Activate() 
{
	if (tData.empty()) 
	{
		GetSystemManager->context->PSSetShaderResources(0, 1, DummyTexture::GetInstance()->GetTexture());
	}
	for (auto& it : tData) 
	{
		GetSystemManager->context->PSSetShaderResources(it.first, 1, &it.second);
	}
}

bool Texture::Create(u_int width, u_int height, DXGI_FORMAT format)
{
	HRESULT hr = S_OK;
	ID3D11Device* device = GetSystemManager->device.Get();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D;

	//	テクスチャ作成
	{
		texture2dDesc = {};
		texture2dDesc.Width = width;
		texture2dDesc.Height = height;
		texture2dDesc.MipLevels = 1;
		texture2dDesc.ArraySize = 1;
		texture2dDesc.Format = format;
		texture2dDesc.SampleDesc.Count = 1;
		texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
		texture2dDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		texture2dDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		hr = device->CreateTexture2D(&texture2dDesc, NULL, Texture2D.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	//	レンダーターゲットビュー作成
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvd = {};
		rtvd.Format = format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvd.Texture2D.MipSlice = 0;
		hr = device->CreateRenderTargetView(Texture2D.Get(), &rtvd, renderTargetView.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	//	シェーダーリソースビュー作成
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MostDetailedMip = 0;
		srvd.Texture2D.MipLevels = 1;
		hr = device->CreateShaderResourceView(Texture2D.Get(), &srvd, shaderResourceView.GetAddressOf());
		assert(SUCCEEDED(hr));
	}
	return true;
}

bool Texture::CreateDepth(u_int width, u_int height, DXGI_FORMAT format)
{
	HRESULT hr = S_OK;
	ID3D11Device* device = GetSystemManager->device.Get();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D;

	//	テクスチャ作成
	{
		texture2dDesc = {};
		texture2dDesc.Width = width;
		texture2dDesc.Height = height;
		texture2dDesc.MipLevels = 1;
		texture2dDesc.ArraySize = 1;
		texture2dDesc.Format = format;
		texture2dDesc.SampleDesc.Count = 1;
		texture2dDesc.SampleDesc.Quality = 0;
		texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
		texture2dDesc.CPUAccessFlags = 0;
		texture2dDesc.MiscFlags = 0;
		texture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		hr = device->CreateTexture2D(&texture2dDesc, NULL, Texture2D.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	// 深度ステンシルビュー設定
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
		dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice = 0;
		hr = device->CreateDepthStencilView(Texture2D.Get(), &dsvd, depthStencilView.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	//	シェーダーリソースビュー作成
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MostDetailedMip = 0;
		srvd.Texture2D.MipLevels = 1;
		hr = device->CreateShaderResourceView(Texture2D.Get(), &srvd, shaderResourceView.GetAddressOf());
		assert(SUCCEEDED(hr));
	}
	return true;
}

bool Texture::CreateCube(u_int width, u_int height, DXGI_FORMAT format, int miplevel)
{
	HRESULT hr = S_OK;
	ID3D11Device* device = GetSystemManager->device.Get();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D;

	//	テクスチャ作成
	{
		texture2dDesc = {};
		texture2dDesc.Width = width;
		texture2dDesc.Height = height;
		texture2dDesc.MipLevels = miplevel;//★
		texture2dDesc.ArraySize = 6;
		texture2dDesc.Format = format;
		texture2dDesc.SampleDesc.Count = 1;
		texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
		//	texture2d_desc.CPUAccessFlags = 0;
		texture2dDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		texture2dDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		//	texture2d_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		texture2dDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;

		hr = device->CreateTexture2D(&texture2dDesc, NULL, Texture2D.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	//	レンダーターゲットビュー作成
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvd = {};
		rtvd.Format = format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;//2Dテクスチャ配列
		rtvd.Texture2DArray.FirstArraySlice = 0;
		rtvd.Texture2DArray.ArraySize = 6;
		rtvd.Texture2DArray.MipSlice = 0;
		hr = device->CreateRenderTargetView(Texture2D.Get(), &rtvd, renderTargetView.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	//	シェーダーリソースビュー作成
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvd.TextureCube.MostDetailedMip = 0;
		srvd.TextureCube.MipLevels = miplevel;
		//srvd.TextureCube.MipLevels = 0;
		hr = device->CreateShaderResourceView(Texture2D.Get(), &srvd, shaderResourceView.GetAddressOf());
		assert(SUCCEEDED(hr));
	}
	return true;

}
bool Texture::CreateCubeDepthStencil(u_int width, u_int height)
{
	HRESULT hr = S_OK;
	ID3D11Device* device = GetSystemManager->device.Get();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D;

	//	テクスチャ作成
	{
		texture2dDesc = {};
		texture2dDesc.Width = width;
		texture2dDesc.Height = height;
		texture2dDesc.MipLevels = 1;
		texture2dDesc.ArraySize = 6;
		texture2dDesc.Format = DXGI_FORMAT_D32_FLOAT;
		texture2dDesc.SampleDesc.Count = 1;
		texture2dDesc.SampleDesc.Quality = 0;
		texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
		texture2dDesc.CPUAccessFlags = 0;
		//	texture2dDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		texture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texture2dDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		hr = device->CreateTexture2D(&texture2dDesc, NULL, Texture2D.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	//	デプスステンシルビュー作成
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
		dsvd.Format = DXGI_FORMAT_D32_FLOAT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;//2Dテクスチャ配列
		dsvd.Texture2DArray.FirstArraySlice = 0;
		dsvd.Texture2DArray.ArraySize = 6;
		dsvd.Texture2DArray.MipSlice = 0;
		dsvd.Flags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		hr = device->CreateDepthStencilView(Texture2D.Get(), NULL, depthStencilView.GetAddressOf());
		assert(SUCCEEDED(hr));
	}
	return true;
}

bool Texture::CreateMipMap(u_int width, u_int height, DXGI_FORMAT format)
{
	HRESULT hr = S_OK;
	ID3D11Device* device = GetSystemManager->device.Get();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D;

	//	テクスチャ作成
	{
		texture2dDesc = {};
		texture2dDesc.Width = width;
		texture2dDesc.Height = height;
		texture2dDesc.MipLevels = 9;
		texture2dDesc.ArraySize = 1;
		texture2dDesc.Format = format;
		texture2dDesc.SampleDesc.Count = 1;
		texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
		texture2dDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		texture2dDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		hr = device->CreateTexture2D(&texture2dDesc, NULL, Texture2D.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	//	レンダーターゲットビュー作成
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvd = {};
		rtvd.Format = format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvd.Texture2D.MipSlice = 0;
		hr = device->CreateRenderTargetView(Texture2D.Get(), &rtvd, renderTargetView.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	//	シェーダーリソースビュー作成
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MostDetailedMip = 0;
		srvd.Texture2D.MipLevels = 9;
		hr = device->CreateShaderResourceView(Texture2D.Get(), &srvd, shaderResourceView.GetAddressOf());
		assert(SUCCEEDED(hr));
	}
	return true;
}

DummyTexture::DummyTexture()
{
	ID3D11Device* device = GetSystemManager->device.Get();

	const int width = 8;
	const int height = 8;
	UINT pixels[width * height];
	::memset(pixels, 0xFF, sizeof(pixels));

	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA data;
	::memset(&data, 0, sizeof(data));
	data.pSysMem = pixels;
	data.SysMemPitch = width;

	Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture;
	device->CreateTexture2D(&desc, &data, texture.GetAddressOf());

	device->CreateShaderResourceView(texture.Get(), nullptr, dummyShaderResourceView.GetAddressOf());
}