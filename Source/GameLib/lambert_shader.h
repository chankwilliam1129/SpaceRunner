#pragma once
#include "shader.h"
#include "system.h"
#include "singleton.h"
#include "../SourceCode/camera.h"
#include "../SourceCode/light.h"

class LambertShader :public Shader
{
public:
	LambertShader(ID3D11Device* device)
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONES",    0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		const char* vsName = "./Data/Shaders/CSO/lambert_vs.cso";
		const char* psName = "./Data/Shaders/CSO/lambert_ps.cso";

		InitShaders(device, layout, ARRAYSIZE(layout), vsName, psName);
	}

	void Begin(ID3D11DeviceContext* context);

	void Draw(ID3D11DeviceContext* context, Model* model, DirectX::XMMATRIX world)
	{
		model->CalculateLocalTransform();
		model->CalculateWorldTransform(world);
		model->Render(context);
	}
};