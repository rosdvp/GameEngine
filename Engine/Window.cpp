#include "pch.h"
#include "Window.h"

#include "Logger.h"

using namespace BlahEngine;

Window* Window::_instance = nullptr;

Window::Window() :
	_inputSystem(nullptr),
	_hwnd(nullptr),
	_isExit(false),
	_isActive(true),
	_isMinimized(false),
	_isMaximized(false),
	_isResized(false)
{
	if (!_instance)
		_instance = this;
	else
		throw std::exception("window already exists");
}

Window::~Window()
{
	if (_hwnd)
		DestroyWindow(_hwnd);
	_hwnd = nullptr;

	Logger::Debug("window", "destroyed");
}

void Window::Create(const WindowDesc& windowDesc)
{
	Logger::Debug("window", "creating");
	_info = windowDesc;

	WNDCLASSEXW wnd;
	
	wnd.style         = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc   = StaticWndProc;
	wnd.cbClsExtra    = 0;
	wnd.cbWndExtra    = 0;
	wnd.hInstance     = nullptr;
	wnd.hIcon         = LoadIcon(nullptr, IDI_WINLOGO);
	wnd.hIconSm       = wnd.hIcon;
	wnd.hCursor       = LoadCursor(0, IDC_ARROW);
	wnd.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wnd.lpszMenuName  = nullptr;
	wnd.lpszClassName = L"D3D11F";
	wnd.cbSize        = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wnd))
		throw std::exception("failed to register window");

	RECT rect = { 0, 0, _info.Width, _info.Height };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

	long width = rect.right - rect.left;
	long height = rect.bottom - rect.top;

	long left = _info.PosX;
	long top  = _info.PosY;

	_hwnd = CreateWindowEx(NULL,
		L"D3D11F",
		_info.Caption.c_str(),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		left, top, width, height,
		nullptr,
		nullptr,
		nullptr,
		nullptr);

	if (!_hwnd)
		throw std::exception("failed to create window");

	ShowWindow(_hwnd, SW_SHOW);
	UpdateWindow(_hwnd);

	Logger::Debug("window", "created");
}

void Window::Run()
{
	MSG msg;
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT Window::WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_CREATE:
		return 0;
	case WM_CLOSE:
		_isExit = true;
		return 0;
	case WM_ACTIVATE:
		if (LOWORD(wParam) != WA_INACTIVE)
			_isActive = true;
		else
			_isActive = false;
		return 0;
	case WM_MOVE:
		_info.PosX = LOWORD(lParam);
		_info.PosY = HIWORD(lParam);
		ReportWindowUpdateToInputSystem();
		return 0;
	case WM_SIZE:
		if (!_info.IsResize)
			return 0;
		_info.Width = LOWORD(lParam);
		_info.Height = HIWORD(lParam);
		_isResized = true;
		if (wParam == SIZE_MINIMIZED)
		{
			_isActive = false;
			_isMinimized = true;
			_isMaximized = false;
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			_isActive = true;
			_isMinimized = false;
			_isMaximized = true;
		}
		else if (wParam == SIZE_RESTORED)
		{
			if (_isMinimized)
			{
				_isActive = true;
				_isMinimized = false;
			}
			else if (_isMaximized)
			{
				_isActive = true;
				_isMaximized = false;
			}
		}
		ReportWindowUpdateToInputSystem();
		return 0; 
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MOUSEWHEEL:
	case WM_KEYDOWN:
	case WM_KEYUP:
		if (_inputSystem)
			_inputSystem->AddEvent(nMsg, wParam, lParam);
		return 0;
	}

	return DefWindowProcW(hwnd, nMsg, wParam, lParam);
}

void Window::AttachInput(InputModule* inputSystem)
{
	_inputSystem = inputSystem;
	ReportWindowUpdateToInputSystem();
}

void Window::ReportWindowUpdateToInputSystem()
{
	RECT rect;
	rect.left = _info.PosX;
	rect.top = _info.PosY;
	rect.right = _info.Width;
	rect.bottom = _info.Height;
	if (_inputSystem)
		_inputSystem->SetWinRect(rect);
}

namespace BlahEngine
{
LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	return Window::GetInstance()->WndProc(hwnd, nMsg, wParam, lParam);
}
}