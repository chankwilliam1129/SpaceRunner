#include "render_texture.h"

RenderTexture::RenderTexture(ID3D11Device* device, float textureWidth, float textureHeight)
{
	//テクスチャー
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	device->CreateTexture2D(&textureDesc, NULL, renderTargetTexture.GetAddressOf());

	//レンダーターゲットビュー
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	device->CreateRenderTargetView(renderTargetTexture.Get(), &renderTargetViewDesc, renderTargetView.GetAddressOf());

	//シェーダーリソースビュー
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(renderTargetTexture.Get(), &shaderResourceViewDesc, shaderResourceView.GetAddressOf());

	//デプステクスチャー
	D3D11_TEXTURE2D_DESC txDesc = {};
	txDesc.Width = textureWidth;
	txDesc.Height = textureHeight;
	txDesc.MipLevels = 1;
	txDesc.ArraySize = 1;
	txDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	txDesc.SampleDesc.Count = 1;
	txDesc.SampleDesc.Quality = 0;
	txDesc.Usage = D3D11_USAGE_DEFAULT;
	txDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	txDesc.CPUAccessFlags = 0;
	txDesc.MiscFlags = 0;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencilTexture;
	device->CreateTexture2D(&txDesc, NULL, &pDepthStencilTexture);

	//デプスステンシルビュー
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	device->CreateDepthStencilView(pDepthStencilTexture.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf());

	//ビューポート
	viewport.Width = (float)textureWidth;
	viewport.Height = (float)textureHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
}

void RenderTexture::SetRenderTarget()
{
	GetSystemManager->context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
	GetSystemManager->context->RSSetViewports(1, &viewport);
	return;
}

void RenderTexture::ClearRenderTarget(float red, float green, float blue, float alpha)
{
	float color[4];
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	GetSystemManager->context.Get()->ClearRenderTargetView(renderTargetView.Get(), color);
	GetSystemManager->context.Get()->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	return;
}

void RenderTexture::Activate() 
{
	GetSystemManager->context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
	GetSystemManager->context->RSSetViewports(1, &viewport);
}

ID3D11ShaderResourceView* RenderTexture::GetShaderResourceView()
{
	return shaderResourceView.Get();
}

D3D11_VIEWPORT* RenderTexture::GetViewPort()
{
	return &viewport;
};