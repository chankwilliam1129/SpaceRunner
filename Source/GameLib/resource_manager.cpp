#include <assert.h>
#include "sprite.h"
#include "resource_manager.h"

static std::map <std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> srvData;
static std::map <const char*, ResourceManager::ResourceVertexShaders>            vsData;
static std::map <const char*, Microsoft::WRL::ComPtr<ID3D11PixelShader>>         psData;

bool loadCSOFile(BYTE** data, long* size, const char* fileName)
{
	FILE* fp = nullptr;
	if (fopen_s(&fp, fileName, "rb")) { return false; }
	fseek(fp, 0, SEEK_END);
	long cso_sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*data = new BYTE[cso_sz];
	size_t s = fread_s(*data, sizeof(BYTE) * cso_sz, sizeof(BYTE), cso_sz, fp);
	fclose(fp);
	if (s != sizeof(BYTE) * cso_sz) { return false; }
	*size = cso_sz;
	return	true;
}

bool ResourceManager::CreateShaderResourceView(ID3D11Device* device, const wchar_t* fileName,
	ID3D11ShaderResourceView** shaderResourceView, D3D11_TEXTURE2D_DESC& texture2dDesc)
{
	HRESULT	hr = S_OK;
	Microsoft::WRL::ComPtr <ID3D11Resource> resource = nullptr;
	auto cache = srvData.find(fileName);
	if (cache == srvData.end())
	{
		hr = DirectX::CreateWICTextureFromFile(device, fileName, &resource, shaderResourceView);
		if (FAILED(hr))assert(0 && "CreateWICTextureFromFileì¬Ž¸”s");		
		srvData.insert(std::make_pair(fileName, *shaderResourceView));
	}
	else
	{
		//*shader_resource_view = SRViewsData[fileName].ShaderResourceView.Get();
		*shaderResourceView = cache->second.Get();
		(*shaderResourceView)->AddRef();
		(*shaderResourceView)->GetResource(resource.GetAddressOf());
	}
	//	texture2d_desc‚ÌŽæ“¾
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d = nullptr;
	hr = resource.Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
	if (FAILED(hr))
	{
		resource->Release();
		(*shaderResourceView)->Release();
		assert(0 && "texture2d_desc‚ÌŽæ“¾Ž¸”s");
	}
	//	texture2d_desc•Û‘¶	
	texture2d->GetDesc(&texture2dDesc);
	return	true;
}

bool ResourceManager::CreateVertexShaders(ID3D11Device* device, const char* fileName,
	D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT numElement, ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout)
{
	HRESULT	hr = S_OK;
	ResourceVertexShaders vs = {};
	auto cache = vsData.find(fileName);
	if (cache == vsData.end())
	{
		BYTE* cso_data = nullptr;
		long cso_sz = {};
		if (!loadCSOFile(&cso_data, &cso_sz, fileName))assert(0 && "csoƒ[ƒhŽ¸”s");

		hr = device->CreateVertexShader(cso_data, cso_sz, NULL, vertexShader);
		if (FAILED(hr))assert(0 && "VertexShaderì¬Ž¸”s");

		hr = device->CreateInputLayout(inputElementDesc, numElement, cso_data, cso_sz, inputLayout);
		if (FAILED(hr)) assert(0 && "InputLayoutì¬Ž¸”s");

		delete[] cso_data;
		vs.vertexShader = *vertexShader;
		vs.inputLayout = *inputLayout;
		vsData.insert(std::make_pair(fileName, vs));
	}
	else
	{
		//*vertexShader = vsData[fileName].vertexShader.Get();
		//*inputLayout = vsData[fileName].inputLayout.Get();
		*vertexShader = cache->second.vertexShader.Get();
		(*vertexShader)->AddRef();
		*inputLayout = cache->second.inputLayout.Get();
		(*inputLayout)->AddRef();
	}
	return true;
}

bool ResourceManager::CreatePixelShaders(
	ID3D11Device* device, const char* fileName, ID3D11PixelShader** pixelShader)
{
	HRESULT	hr = S_OK;
	auto cache = psData.find(fileName);
	if (cache == psData.end())
	{
		BYTE* cso_data = nullptr;
		long cso_sz = {};
		if (!loadCSOFile(&cso_data, &cso_sz, fileName))assert(0 && "csoƒ[ƒhŽ¸”s");

		hr = device->CreatePixelShader(cso_data, cso_sz, NULL, pixelShader);
		if (FAILED(hr)) assert(0 && "CreatePixelShaderì¬Ž¸”s");
		delete[] cso_data;	
		psData.insert(std::make_pair(fileName, *pixelShader));
	}
	else
	{
		//*pixelShader = psData[fileName].pixelShader.Get();
		*pixelShader = cache->second.Get();
		(*pixelShader)->AddRef();
	}
	return true;
}