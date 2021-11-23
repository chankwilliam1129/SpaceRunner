#pragma once
#include "resource_manager.h"
#include "singleton.h"
#include "system.h"

class Texture
{
public:
	std::map<int, ID3D11ShaderResourceView*>tData;

public:
	Texture() {};
	virtual ~Texture() {};

	void Load(const wchar_t* file_name);
	void Load(ID3D11ShaderResourceView* t_shaderResourceView);
	//bool LoadMipMap(const wchar_t* filename = nullptr);

	void Set(UINT Slot = 0, BOOL flg = TRUE);

	void Activate();

	bool Create(u_int width, u_int height, DXGI_FORMAT format);
	bool CreateMipMap(u_int width, u_int height, DXGI_FORMAT format);

	// 深度ステンシルバッファ作成
	bool CreateDepth(u_int width, u_int height, DXGI_FORMAT format = DXGI_FORMAT_R24G8_TYPELESS);

	//CubeMap用
	bool CreateCube(u_int width, u_int height, DXGI_FORMAT format, int miplevel = 1);
	bool CreateCubeDepthStencil(u_int width, u_int height);

	ID3D11ShaderResourceView* GetTexture() const { return shaderResourceView.Get(); }
	ID3D11RenderTargetView* GetRenderTargetView()const { return renderTargetView.Get(); }
	ID3D11DepthStencilView* GetDepthStencilView() const { return depthStencilView.Get(); }
	D3D11_TEXTURE2D_DESC* GetTexture2D() { return &texture2dDesc; }

protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   renderTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>   depthStencilView = nullptr;
	D3D11_TEXTURE2D_DESC texture2dDesc = {};
};

class DummyTexture :public Singleton<DummyTexture>
{
public:
	DummyTexture();
	ID3D11ShaderResourceView** GetTexture() { return dummyShaderResourceView.GetAddressOf(); }

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> dummyShaderResourceView;
};


class TextureManager : public Singleton<TextureManager> 
{
public:
	std::map<std::string, ID3D11ShaderResourceView*, std::less<>>tData;

public:
	TextureManager() { Create(); };

	void Initialize()
	{
		Create();
	}

	void Create();

private:
	ID3D11ShaderResourceView* Load(const wchar_t* filename) 
	{
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
		D3D11_TEXTURE2D_DESC  texture2dDesc = {};
		ResourceManager::CreateShaderResourceView(GetSystemManager->device.Get(), filename, shaderResourceView.GetAddressOf(), texture2dDesc);
		return shaderResourceView.Get();
	}
};