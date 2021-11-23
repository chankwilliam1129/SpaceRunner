#include <ctime>
#include "system.h"
#include "frame_rate.h"
#include "debug.h"
#include "input.h"
#include "font.h"
#include "sound.h"
#include "shader.h"

#include "../SourceCode/load_mesh.h"

// SystemManager
void SystemManager::Initialize(double frameRate, int width, int height, bool isFullscreen)
{
	srand((unsigned int)time(NULL));

#ifdef _DEBUG
	isFullscreen = false;
#endif

	//サウンドの初期設定
	SoundManager::GetInstance()->Initialize();

	// DirectX11の初期化
	DirectX11Manager::GetInstance()->Initialize(hwnd, width, height, isFullscreen);

	// フレームレートの設定
	if (frameRate) { hrTimer.SetFrameRate(frameRate); }

	//フォントの初期設定
	FontManager::GetInstance()->font = std::make_unique<Font>(device.Get(), "Data/Fonts/MS Gothic.fnt", 1024);

	//メッシュのロード
	MeshManager::GetInstance()->Load();

	//シェーダーの初期化
	ShaderManager::GetInstance()->Initialize();

#ifdef USE_IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	//日本語用フォントの設定
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 13.0f, nullptr, glyphRangesJapanese);

	ImGui_ImplWin32_Init(GetSystemManager->hwnd);
	ImGui_ImplDX11_Init(GetSystemManager->device.Get(), GetSystemManager->context.Get());
	ImGui::StyleColorsDark();
#endif 
}

void SystemManager::Release()
{
	MeshManager::GetInstance()->Release();
	ShaderManager::GetInstance()->Release();
	DirectX11Manager::GetInstance()->Release();
	WindowManager::GetInstance()->Release();
}

HRESULT SystemManager::SetPresent(UINT SyncInterval, UINT Flags)
{
	return swapChain->Present(SyncInterval, Flags);
}

void WindowManager::Release()
{
	UnregisterClass(CLASS_NAME, GetModuleHandle(NULL));
}

void WindowManager::ClearRenderTarget(const Vector4& color)
{
	GetSystemManager->context->ClearRenderTargetView(GetSystemManager->renderTargetView.Get(), (const float*)&color);
	GetSystemManager->context->ClearDepthStencilView(GetSystemManager->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void WindowManager::ClearRenderTarget(float r, float g, float b)
{
	ClearRenderTarget(Vector4(r, g, b, 1.0f));
}

void WindowManager::SetRenderTarget()
{
	GetSystemManager->context->OMSetRenderTargets(1, GetSystemManager->renderTargetView.GetAddressOf(), GetSystemManager->depthStencilView.Get());
}

// DirectX11Manager
bool DirectX11Manager::Initialize(HWND hwnd, int width, int height, bool isFullscreen)
{
	HRESULT hr = S_OK;

	DXGI_SWAP_CHAIN_DESC swapChainDesc               = {};
	swapChainDesc.BufferCount                        = 1;
	swapChainDesc.BufferDesc.Width                   = width;
	swapChainDesc.BufferDesc.Height                  = height;
	swapChainDesc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator   = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SampleDesc.Count                   = 1;
	swapChainDesc.SampleDesc.Quality                 = 0;
	swapChainDesc.Windowed                           = !isFullscreen;
	swapChainDesc.OutputWindow                       = hwnd;

	D3D_FEATURE_LEVEL featureLevel = {};

	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,			              // ビデオアダプタ指定(nullptrは既定)
		D3D_DRIVER_TYPE_HARDWARE,         // ドライバのタイプ
		nullptr,			              // D3D_DRIVER_TYPE_SOFTWARE指定時に使用
		0,			  	                  // フラグ指定
		nullptr,			              // D3D_FEATURE_LEVEL指定で自分で定義した配列を指定可能
		0,				                  // 上のD3D_FEATURE_LEVEL配列の要素数
		D3D11_SDK_VERSION,	              // SDKバージョン
		&swapChainDesc,	                  // DXGI_SWAP_CHAIN_DESC
		GetSystemManager->swapChain.GetAddressOf(),// 関数成功時のSwapChainの出力先 
		GetSystemManager->device.GetAddressOf(),	  // 関数成功時のDeviceの出力先
		&featureLevel,		              // 成功したD3D_FEATURE_LEVELの出力先
		GetSystemManager->context.GetAddressOf()	  // 関数成功時のContextの出力先	
	);
	if (FAILED(hr)) return false;

	D3D11_TEXTURE2D_DESC backBufferDesc = {};
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer = nullptr;
		hr = GetSystemManager->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)backBuffer.GetAddressOf());
		if (FAILED(hr)) return false;

		hr = GetSystemManager->device->CreateRenderTargetView(backBuffer.Get(), NULL, GetSystemManager->renderTargetView.GetAddressOf());
		if (FAILED(hr)) return false;
		backBuffer->Release();
	}

	D3D11_TEXTURE2D_DESC depthStencilBufferDesc = backBufferDesc;
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencilTexture = nullptr;
		depthStencilBufferDesc.Width              = width;
		depthStencilBufferDesc.Height             = height;
		depthStencilBufferDesc.MipLevels          = 1;
		depthStencilBufferDesc.ArraySize          = 1;
		depthStencilBufferDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilBufferDesc.SampleDesc.Count   = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
		depthStencilBufferDesc.Usage              = D3D11_USAGE_DEFAULT;
		depthStencilBufferDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
		depthStencilBufferDesc.CPUAccessFlags     = 0;
		depthStencilBufferDesc.MiscFlags          = 0;

		hr = GetSystemManager->device->CreateTexture2D(&depthStencilBufferDesc, NULL, pDepthStencilTexture.GetAddressOf());
		if (FAILED(hr)) return false;

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format             = depthStencilBufferDesc.Format;
		depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		hr = GetSystemManager->device->CreateDepthStencilView(pDepthStencilTexture.Get(), &depthStencilViewDesc, GetSystemManager->depthStencilView.GetAddressOf());
		pDepthStencilTexture->Release();
		if (FAILED(hr)) return false;
	}

	Rasterizer::Initialize(GetSystemManager->device.Get(), GetSystemManager->context.Get());
	DepthStencil::Initialize(GetSystemManager->device.Get(), GetSystemManager->context.Get());
	Sampler::Initialize(GetSystemManager->device.Get(), GetSystemManager->context.Get());
	Blender::Initialize(GetSystemManager->device.Get(), GetSystemManager->context.Get());
	DebugManager::GetInstance()->SetDebug(L"./Data/Fonts/font0.png", Debug::INSTANCE_NUM, height);
	ViewPortManager::GetInstance()->SetViewPort(width, height, ViewPortManager::ViewType::Nomal);

	GetSystemManager->context->OMSetRenderTargets(1, GetSystemManager->renderTargetView.GetAddressOf(), GetSystemManager->depthStencilView.Get());
	return true;
}

void DirectX11Manager::Release()
{
	DebugManager::GetInstance()->Release();
	//if (GetSystemManager->m.blender) GetSystemManager->m.blender->Release();

	if (GetSystemManager->depthStencilView.Get())	GetSystemManager->depthStencilView->Release();
	if (GetSystemManager->renderTargetView.Get())	GetSystemManager->renderTargetView->Release();
	if (GetSystemManager->swapChain.Get())		    GetSystemManager->swapChain->Release();
	if (GetSystemManager->context.Get())			GetSystemManager->context->Release();
	if (GetSystemManager->device.Get())		    GetSystemManager->device->Release();

	GetSystemManager->swapChain->SetFullscreenState(false, nullptr);
	Rasterizer::Release();
	DepthStencil::Release();
	Sampler::Release();
	Blender::Release();
}

void ViewPortManager::SetViewPorts(UINT NumViewports, const D3D11_VIEWPORT* pViewports)
{
	GetSystemManager->context->RSSetViewports(NumViewports, pViewports);
}

void ViewPortManager::SetViewPort(int width, int height, int state)
{
	switch (state)
	{
	case ViewType::Nomal:
		viewPorts[0].Width    = (FLOAT)width;  //画面の横サイズ
		viewPorts[0].Height   = (FLOAT)height; //画面の縦サイズ
		viewPorts[0].TopLeftX = 0.0f;          //画面左上のx座標
		viewPorts[0].TopLeftY = 0.0f;          //画面左上のy座標
		viewPorts[0].MinDepth = 0.0f;          //深度値の最小値
		viewPorts[0].MaxDepth = 1.0f;	       //深度値の最大値
		screenWidth  = (FLOAT)width;
		screenHeight = (FLOAT)height;
		screenNum = 1;
		break;

	case ViewType::Y:
		//上画面
		viewPorts[0].Width    = (FLOAT)width;
		viewPorts[0].Height   = (FLOAT)height / 2;
		viewPorts[0].TopLeftX = 0.0f;
		viewPorts[0].TopLeftY = 0.0f;
		viewPorts[0].MinDepth = 0.0f;
		viewPorts[0].MaxDepth = 1.0f;

		//下画面
		viewPorts[1].Width    = (FLOAT)width;
		viewPorts[1].Height   = (FLOAT)height / 2;
		viewPorts[1].TopLeftX = 0.0f;
		viewPorts[1].TopLeftY = (FLOAT)height / 2;
		viewPorts[1].MinDepth = 0.0f;
		viewPorts[1].MaxDepth = 1.0f;
		screenWidth   = (FLOAT)width;
		screenHeight  = (FLOAT)height / 2;
		screenNum = 2;
		break;

	case ViewType::X:
		//左画面
		viewPorts[0].Width    = (FLOAT)width / 2;
		viewPorts[0].Height   = (FLOAT)height;
		viewPorts[0].TopLeftX = 0.0f;
		viewPorts[0].TopLeftY = 0.0f;
		viewPorts[0].MinDepth = 0.0f;
		viewPorts[0].MaxDepth = 1.0f;

		//右画面
		viewPorts[1].Width    = (FLOAT)width / 2;
		viewPorts[1].Height   = (FLOAT)height;
		viewPorts[1].TopLeftX = (FLOAT)width / 2;
		viewPorts[1].TopLeftY = 0.0f;
		viewPorts[1].MinDepth = 0.0f;
		viewPorts[1].MaxDepth = 1.0f;
		screenWidth  = (FLOAT)width / 2;
		screenHeight = (FLOAT)height;
		screenNum = 2;
		break;

	case ViewType::Multi:
		//左上画面
		viewPorts[0].Width    = (FLOAT)width  / 2;
		viewPorts[0].Height   = (FLOAT)height / 2;
		viewPorts[0].TopLeftX = 0.0f;
		viewPorts[0].TopLeftY = 0.0f;
		viewPorts[0].MinDepth = 0.0f;
		viewPorts[0].MaxDepth = 1.0f;

		//右上画面
		viewPorts[1].Width    = (FLOAT)width  / 2;
		viewPorts[1].Height   = (FLOAT)height / 2;
		viewPorts[1].TopLeftX = (FLOAT)width  / 2;
		viewPorts[1].TopLeftY = 0.0f;
		viewPorts[1].MinDepth = 0.0f;
		viewPorts[1].MaxDepth = 1.0f;

		//左下画面
		viewPorts[2].Width    = (FLOAT)width  / 2;
		viewPorts[2].Height   = (FLOAT)height / 2;
		viewPorts[2].TopLeftX = 0.0f;
		viewPorts[2].TopLeftY = (FLOAT)height / 2;
		viewPorts[2].MinDepth = 0.0f;
		viewPorts[2].MaxDepth = 1.0f;

		//右下画面
		viewPorts[3].Width    = (FLOAT)width  / 2;
		viewPorts[3].Height   = (FLOAT)height / 2;
		viewPorts[3].TopLeftX = (FLOAT)width  / 2;
		viewPorts[3].TopLeftY = (FLOAT)height / 2;
		viewPorts[3].MinDepth = 0.0f;
		viewPorts[3].MaxDepth = 1.0f;
		screenWidth  = (FLOAT)width  / 2;
		screenHeight = (FLOAT)height / 2;
		screenNum = 4;
		break;
	}
}
