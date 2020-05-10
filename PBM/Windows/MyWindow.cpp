#include "../stdafx.h"
#include "MyWindow.h"
#include "RenderSetting.h"
#include "../Resource.h"
#include "../Game/World.h"

#include <fcntl.h>
#include <io.h>

ATOM MyWindow::MyRegisterClass()
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PBM));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDI_PBM);
	wcex.lpszClassName = windowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);

}

BOOL MyWindow::InitInstance(int nCmdShow)
{
	HWND hWnd = CreateWindowW(windowClass, title, WS_OVERLAPPEDWINDOW,
		100, 100, (int)width, (int)height, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd) {
		std::cout << "窗口句柄创建失败" << std::endl;
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	// 交换链
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = (UINT)width;
	sd.BufferDesc.Height = (UINT)height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_0;
	HRESULT ret;
	ret = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG,
		&featureLevels, 1, D3D11_SDK_VERSION,
		&sd, &swapChain, &device,
		NULL, &immediateContext);
	if (FAILED(ret)) {
		std::cout << "渲染设备和交换链创建失败" << std::endl;
		return FALSE;
	}

	RenderSetting::InitSetting(device, immediateContext);

	ID3D11Texture2D *pBackBuffer;
	ret = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(ret)) return FALSE;

	ret = device->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);
	if (FAILED(ret)) return FALSE;
	//深度缓冲区
	ID3D11Texture2D *depthStencilBuffer;
	D3D11_TEXTURE2D_DESC depthSencilDesc;
	depthSencilDesc.Width = (UINT)width;
	depthSencilDesc.Height = (UINT)height;
	depthSencilDesc.MipLevels = 1;
	depthSencilDesc.ArraySize = 1;
	depthSencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthSencilDesc.SampleDesc.Count = 1;
	depthSencilDesc.SampleDesc.Quality = 0;
	depthSencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthSencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthSencilDesc.CPUAccessFlags = 0;
	depthSencilDesc.MiscFlags = 0;
	ret = device->CreateTexture2D(&depthSencilDesc, nullptr, &depthStencilBuffer);
	if (FAILED(ret)) return FALSE;
	ret = device->CreateDepthStencilView(depthStencilBuffer, nullptr, &depthStencilView);
	if (FAILED(ret)) return FALSE;

	immediateContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	//D3D11_DEPTH_STENCIL_DESC dsDesc;
	//// Depth test parameters
	//dsDesc.DepthEnable = true;
	//dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//ID3D11DepthStencilState * pDSState;
	//device->CreateDepthStencilState(&dsDesc, &pDSState);
	//immediateContext->OMSetDepthStencilState(pDSState, 1);

	//设置视口映射裁剪空间坐标系
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	immediateContext->RSSetViewports(1, &vp);
	immediateContext->RSSetState(RenderSetting::RSWireframe);

	m_Mouse.Init(hWnd);

	auto world = World::GetInstance();
	world->m_pSwapChain = swapChain;
	world->m_pD3DDevice = device;
	world->m_pImmediateContext = immediateContext;
	world->m_pRenderTargetView = renderTargetView;
	world->m_pDepthStencilView = depthStencilView;
	world->m_pKeyboard = &m_Keyboard;
	world->m_pMouse = &m_Mouse;

	return true;
}

int MyWindow::Run()
{
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PBM));
	MSG msg = { 0 };
	World* gameWorld = World::GetInstance();
	gameWorld->InitResource();
	double mspf = 1000.0 / 30.0;
	double interval;
	// 主消息循环:
	while (WM_QUIT != msg.message)
	{
		ULONGLONG startTime;
		ULONGLONG endTime;
		QueryInterruptTime(&startTime);
		int countOfMessage = 0;
		// 踩坑日记 2020/2/22：
		// 这里使用while循环，而不使用if是一本学问。
		// 使用while的意图是在每一帧开始之前处理所有收到的Message事件
		// 如果使用if的话，会造成一帧只处理一个事件，这样会导致Message事件处理不过来，一直堆积着。
		// 当同时按键盘 和 移动鼠标时 一帧累计的Message事件大约有5~6个。
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			countOfMessage++;
			TranslateMessage(&msg);
			DispatchMessage(&msg); //将消息分发给相应的窗口进程
		}
		//if(countOfMessage > 2)
		//	cout << "Message Count" << countOfMessage << endl;
		if (!gameWorld->Run())
			break;
		//帧率控制
		QueryInterruptTime(&endTime);
		interval = (endTime - startTime) / 10000.0;
		cout << "\rframe time usage: " << interval << "ms  ";
		if (interval > 0.0 && interval < mspf)
			Sleep((DWORD)(mspf - interval));
#if _DEBUG
		else if (interval > 1000)
			continue;// DEBUG 模式时间过长时 忽略当前帧时间
#endif
		QueryInterruptTime(&endTime);
		gameWorld->m_DeltaTime = (endTime - startTime) / 10000.0;
	}
	return (int)msg.wParam;
}

MyWindow::MyWindow(HINSTANCE hInstance) :width(500.0f), height(500.0f)
{
	memset(&m_Info, 0, sizeof(WindowInfo));
	this->hInstance = hInstance;
	LoadStringW(hInstance, IDS_APP_TITLE, title, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PBM, windowClass, MAX_LOADSTRING);
	MyRegisterClass();
	applicationHandle = this;
}

MyWindow::~MyWindow() {}

void MyWindow::Active(bool active)
{
	if (active == m_Info.Active)
		return;
	m_Info.Active = active;
	if (m_Mouse.IsTrackingStarted()) {
		if (active) {
			m_Mouse.ResumeTracking();
		}
		else {
			m_Mouse.PauseTracking();
		}
	}
}

void MyWindow::Moving(const RECT & rect)
{
	if (m_Mouse.IsTrackingStarted()) {
		m_Mouse.PauseTracking();
	}
}

void MyWindow::Moved(int screen_x, int screen_y)
{
	if (m_Mouse.IsTrackingStarted()) {
		m_Mouse.ResumeTracking();
	}
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(applicationHandle->hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN: {
		KeyCode keyCode = LOBYTE(wParam);
		//cout << InterpretKeyCode(keyCode) << " is down" << endl;
		applicationHandle->m_Keyboard.SetKeyState(keyCode, KEY_STATE_DOWN);
		break;
	}
	case WM_ACTIVATE: {
		auto activeCode = LOBYTE(wParam);
		switch (activeCode) {
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			cout << "windows active" << endl;
			applicationHandle->Active(true);
			break;
		case WA_INACTIVE:
			cout << "windows inactive" << endl;
			applicationHandle->Active(false);
			break;
		}
		break;
	}
	case WM_MOVING:
	{
		RECT& rect = *(RECT*)lParam;
		applicationHandle->Moving(rect);
		break;
	}
	case WM_MOVE:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		applicationHandle->Moved(xPos, yPos);
		break;
	}
	case WM_KEYUP: {
		KeyCode keyCode = LOBYTE(wParam);
		//cout << InterpretKeyCode(keyCode) << " is up" << endl;
		applicationHandle->m_Keyboard.SetKeyState(keyCode, KEY_STATE_UP);
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (hWnd == GetForegroundWindow())
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			Mouse& mouse = applicationHandle->m_Mouse;
			mouse.MoveUpdate(xPos, yPos);
		}
		break;
	}
	case  WM_MOUSEWHEEL:
	{
		if (hWnd == GetForegroundWindow())
		{
			float wheelDistance = ((short)HIWORD(wParam)) / (float)WHEEL_DELTA;
			Mouse& mouse = applicationHandle->m_Mouse;
			mouse.WheelUpdate(wheelDistance);
		}
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
