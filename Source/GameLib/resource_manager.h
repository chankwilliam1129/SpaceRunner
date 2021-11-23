#pragma once
#include <d3d11.h>
#include <map>
#include <string>
#include <wrl.h>
#include <WICTextureLoader.h>

bool loadCSOFile(BYTE** data, long* size, const char* fileName);

class ResourceManager
{
public:
	struct ResourceVertexShaders
	{
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader = nullptr;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>  inputLayout  = nullptr;
	};

public:
	static bool CreateShaderResourceView(ID3D11Device*, const wchar_t*, ID3D11ShaderResourceView**, D3D11_TEXTURE2D_DESC&);
	static bool CreateVertexShaders(ID3D11Device*, const char*, D3D11_INPUT_ELEMENT_DESC*, UINT, ID3D11VertexShader**, ID3D11InputLayout**);
	static bool CreatePixelShaders(ID3D11Device*, const char*, ID3D11PixelShader**);
};