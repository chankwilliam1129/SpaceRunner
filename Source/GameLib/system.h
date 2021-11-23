#pragma once
#include <sstream>
#include <d3d11.h>
#include <wrl.h>

#include "high_resolution_timer.h"
#include "blender.h"
#include "rasterizer.h"
#include "depth_stencil.h"
#include "sampler.h"
#include "imgui.h"
#include "vector.h"
#include "singleton.h"

static const LPCWSTR CLASS_NAME = L"3dgp";
static const LPCTSTR TITLE_NAME = L"ÉQÅ[ÉÄêßçÏ";
const LONG SCREEN_WIDTH = 1280;
const LONG SCREEN_HEIGHT = 720;

class SystemManager :public Singleton<SystemManager>
{
public:
	HWND hwnd = {};
	Microsoft::WRL::ComPtr<ID3D11Device>           device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>    context = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain>         swapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;

	HighResolutionTimer hrTimer = {};

public:
	void Initialize(double frameRate, int width = (640), int height = (480), bool isFullscreen = (false));
	void Release();

public:
	HRESULT SetPresent(UINT SyncInterval, UINT Flags);
};

class WindowManager :public Singleton<WindowManager>
{
public:
	void Release();

public:
	void ClearRenderTarget(const Vector4& color);
	void ClearRenderTarget(float r, float g, float b);
	void SetRenderTarget();
};

class DirectX11Manager :public Singleton<DirectX11Manager>
{
public:
	bool Initialize(HWND hwnd, int width, int height, bool isFullscreen);
	void Release();
};

class ViewPortManager :public Singleton<ViewPortManager>
{
public:
	enum ViewType 
	{ 
		Nomal,
		Y, 
		X, 
		Multi
	};

	static const int ViewMax = 4;	
	D3D11_VIEWPORT viewPorts[ViewMax] = { 0 };
	float screenWidth  = 0;
	float screenHeight = 0;
	int   screenNum    = 1;

public:
	void SetViewPorts(UINT NumViewports, const D3D11_VIEWPORT* pViewports);
	void SetViewPort(int width, int height, int state);
};

#define GetSystemManager SystemManager::GetInstance()