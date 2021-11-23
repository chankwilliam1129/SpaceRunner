#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <map>
#include "singleton.h"
#include <sstream>
#include "system.h"


const LONG H_SCREEN_WIDTH = 1280 / 2;
const LONG H_SCREEN_HEIGHT = 720 / 2;

const LONG Q_SCREEN_WIDTH = 1280 / 4;
const LONG Q_SCREEN_HEIGHT = 720 / 4;

class RenderTexture
{
public:
	RenderTexture(ID3D11Device*, float, float);
	void SetRenderTarget();
	void ClearRenderTarget(float, float, float, float);
	void Activate();

	ID3D11ShaderResourceView* GetShaderResourceView();
	D3D11_VIEWPORT* GetViewPort();


private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>          renderTargetTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   renderTargetView    = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView  = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>   depthStencilView    = nullptr;
	D3D11_VIEWPORT viewport;
};