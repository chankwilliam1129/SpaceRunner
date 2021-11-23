#pragma once
#include <list>
#include "../SourceCode/camera.h"
#include "../SourceCode/light.h"
#include "mesh.h"
#include "resource_manager.h"

class Shader
{
public:
	std::map<int, Cbuffer*> cbData;

public:
	virtual void Activate(ID3D11DeviceContext* context) {};
};

template<class T>
class VertexShader :public Shader
{
public:
	VertexShader(const char* cso)
	{
		size_t numElements;
		const D3D11_INPUT_ELEMENT_DESC* inputElementDesc = T::CreateInputElementDesc(numElements);
		if (cso)
		{ 
			ResourceManager::CreateVertexShaders(GetSystemManager->device.Get(), cso, const_cast<D3D11_INPUT_ELEMENT_DESC*>(inputElementDesc), numElements, shaderObject.GetAddressOf(), inputLayout.GetAddressOf()); 
		}
	}

	void Activate(ID3D11DeviceContext* context)
	{
		for (auto& it : cbData) 
		{
			it.second->Set(context, it.first, true, false);
		}
		context->IASetInputLayout(inputLayout.Get());
		context->VSSetShader(shaderObject.Get(), 0, 0);
	}

public:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shaderObject;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

class PixelShader :public Shader
{
public:
	PixelShader(const char* cso)
	{
		if (cso) 
		{ 
			ResourceManager::CreatePixelShaders(GetSystemManager->device.Get(), cso, shaderObject.GetAddressOf());
		}
	}

	void Activate(ID3D11DeviceContext* context)
	{
		for (auto& it : cbData)
		{
			it.second->Activate(context, it.first, false, true);
		}
		context->PSSetShader(shaderObject.Get(), 0, 0);
	}

public:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shaderObject;
};

class ShaderManager :public Singleton<ShaderManager> 
{
public:
	std::map<std::string, Shader*, std::less<>>vsData;
	std::map<std::string, Shader*, std::less<>>psData;

public:
	void Initialize()
	{
		CreateVs();
		CreatePs();
	}

	void CreateVs() 
	{
		vsData["Mesh"] = new VertexShader<Mesh>("./Data/Shaders/CSO/mesh_vs.cso");
		vsData["SkinMesh"] = new VertexShader<SkinMesh>("./Data/Shaders/CSO/skin_mesh_vs.cso");
		vsData["Sprite"] = new VertexShader<Sprite>("./Data/Shaders/CSO/sprite_vs.cso");
		vsData["HorizontalBlur"] = new VertexShader<Sprite>("./Data/Shaders/CSO/horizontal_blur_vs.cso");
		vsData["VeticalBlur"] = new VertexShader<Sprite>("./Data/Shaders/CSO/vertical_blur_vs.cso");
		vsData["Texture"] = new VertexShader<Mesh>("./Data/Shaders/CSO/texture_vs.cso");
	}

	void CreatePs() 
	{
		psData["Mesh"] = new PixelShader("./Data/Shaders/CSO/mesh_ps.cso");
		psData["Sprite"] = new PixelShader("./Data/Shaders/CSO/sprite_ps.cso");
		psData["HorizontalBlur"] = new PixelShader("./Data/Shaders/CSO/horizontal_blur_ps.cso");
		psData["VeticalBlur"] = new PixelShader("./Data/Shaders/CSO/vertical_blur_ps.cso");
		psData["GausianHorizontalBlur"] = new PixelShader("./Data/Shaders/CSO/gausian_horizontal_blur_ps.cso");
		psData["GausianVeticalBlur"] = new PixelShader("./Data/Shaders/CSO/gausian_vertical_blur_ps.cso");
		psData["GausianBlur"] = new PixelShader("./Data/Shaders/CSO/gausian_blur_ps.cso");
		psData["Glow"] = new PixelShader("./Data/Shaders/CSO/glow_ps.cso");
		psData["Glow2"] = new PixelShader("./Data/Shaders/CSO/glow_diffuse_ps.cso");
	}

	void Release() 
	{
		vsData.clear();
		psData.clear();
	}
};

