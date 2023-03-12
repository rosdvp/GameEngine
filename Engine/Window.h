#pragma once

#include "InputModule.h"

namespace BlahEngine
{
struct WindowDesc
{
	WindowDesc() :
		PosX(200),
		PosY(20),
		Caption(L""),
		Width(640),
		Height(480),
		IsResize(true) {}

	int PosX;
	int PosY;
	std::wstring Caption;
	int Width;
	int Height;
	bool IsResize;
};

class Window
{
public:
	Window();
	~Window();

	static Window* GetInstance() { return _instance; }

	void Create(const WindowDesc& windowDesc);

	void Run();

	void AttachInput(InputModule* inputSystem);

	HWND GetHWND() const { return _hwnd; }

	int GetLeft() const { return _info.PosX; }
	int GetTop() const { return _info.PosY; }
	int GetWidth() const { return _info.Width; }
	int GetHeight() const { return _info.Height; }

	const std::wstring& GetCaption() const { return _info.Caption; }

	bool IsExit() const { return _isExit; }

	bool IsActive() const { return _isActive; }

	bool IsResize()
	{
		bool ret   = _isResized;
		_isResized = false;
		return ret;
	}

	LRESULT WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
private:
	static Window* _instance;

	void ReportWindowUpdateToInputSystem();

	WindowDesc _info;
	InputModule* _inputSystem;
	HWND _hwnd;
	bool _isExit;
	bool _isActive;
	bool _isMinimized;
	bool _isMaximized;
	bool _isResized;
};

static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
}
