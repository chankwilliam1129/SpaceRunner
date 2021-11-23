#include <windows.h>
#include <memory>
#include <assert.h>
#include <tchar.h>
#include "../GameLib/frame_rate.h"
#include "scene_title.h"
#include "time.h"

LRESULT CALLBACK fnWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
#ifdef USE_IMGUI
	//imgui event catch
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) { return true; }
#endif

	Mouse::GetState(msg);
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CREATE:
		break;

	case WM_KEYDOWN:
		//ESCAPEキーが押されたら強制終了
		if (wparam == VK_ESCAPE)PostMessage(hwnd, WM_CLOSE, 0, 0);
		break;

	case WM_ENTERSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		GetSystemManager->hrTimer.Stop();
		break;

	case WM_EXITSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		GetSystemManager->hrTimer.Start();
		break;

	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR cmd_line, INT cmd_show)
{
	MSG hMsg = { 0 };

	// ウインドウの初期設定
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	WNDCLASSEX wcex    = {};
	wcex.cbSize        = sizeof(WNDCLASSEX);
	wcex.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc   = fnWndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = instance;
	wcex.hIcon         = NULL;
	wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName  = NULL;
	wcex.lpszClassName = CLASS_NAME;
	wcex.hIconSm       = 0;
	RegisterClassEx(&wcex);

	RECT rect;
	::SetRect(&rect, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	GetSystemManager->hwnd = CreateWindow
	(
		CLASS_NAME,
		TITLE_NAME,
		WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, windowWidth, windowHeight,
		NULL,
		NULL,
		instance,
		NULL);

	ShowWindow(GetSystemManager->hwnd, cmd_show);

	bool isShowFrameRate = true;
	bool isFullScreen = false;
	double fixedFrame = 60.0f;

	GetTimeManager->SetFrame(fixedFrame);
	GetSystemManager->Initialize(
		fixedFrame,
		static_cast<int>(SCREEN_WIDTH),
		static_cast<int>(SCREEN_HEIGHT),
		isFullScreen
	);

	ShowCursor(!isFullScreen);	// フルスクリーン時はカーソルを消す

	SceneManager::GetInstance()->ChangeScene(std::make_unique<SceneTitle>());

	while (hMsg.message != WM_QUIT) 
	{
		if (PeekMessage(&hMsg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&hMsg);
			DispatchMessage(&hMsg);
		}
		else
		{
			while (!GetSystemManager->hrTimer.Tick());

			if (isShowFrameRate)
			{
				CalculateFrameStats(GetSystemManager->hwnd, &GetSystemManager->hrTimer); // フレームレート計算・タイトルバーに表示
			}

			SceneManager::GetInstance()->Update();
			SceneManager::GetInstance()->Render();
		}
	}

	GetSystemManager->Release();

#ifdef USE_IMGUI
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif 
	return 0;
}
