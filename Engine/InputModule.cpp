#include "pch.h"
#include "InputModule.h"

#include "Logger.h"

using namespace BlahEngine;

InputModule::InputModule() :
	_winRect(), _mouseX(0), _mouseY(0),
	_isMouseWheelChanged(0), _mouseWheel(0),
	_events(10),
	_keysDownMap(nullptr) {}

InputModule::~InputModule()
{
	delete[] _keysDownMap;
	Logger::Debug("input module", "destroyed");
}

void InputModule::Init()
{
	_keysDownMap = new bool[KEY_MAX];
	for (int i = 0; i < KEY_MAX; i++)
		_keysDownMap[i] = false;
	
	POINT mousePos;
	GetCursorPos(&mousePos);
	_mouseX = mousePos.x;
	_mouseY = mousePos.y;
	_mousePrevX = _mouseX;
	_mousePrevY = _mouseY;

	Logger::Debug("input module", "initialized");
}

void InputModule::Run()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	_mouseX = mousePos.x - _winRect.left;
	_mouseY = mousePos.y - _winRect.top;
}


void InputModule::AddEvent(const UINT& msg, WPARAM wParam, LPARAM lParam)
{
	EKeyCode key;
	bool isDown;
	wchar_t buffer[1]{ ' ' };

	DecodeKey(msg, wParam, key, isDown);

	if (isDown && _keysDownMap[key])
		return;
	_keysDownMap[key] = isDown;

	if (msg == WM_KEYDOWN || msg == WM_KEYUP)
	{
		BYTE keyState[256];
		GetKeyboardState(keyState);
		ToUnicode(wParam, HIWORD(lParam) & 0xFF, keyState, buffer, 1, 0);
	}

	_events.TryAdd(key, isDown, buffer[0]);

	//_isMouseWheelChanged = true;
	//_mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
}

void InputModule::ClearEvents()
{
	_mousePrevX = _mouseX;
	_mousePrevY = _mouseY;

	_isMouseWheelChanged = false;
	_events.Clear();
}

void InputModule::SetWinRect(const RECT& winRect)
{
	_winRect.left = winRect.left;
	_winRect.right = winRect.right;
	_winRect.top = winRect.top;
	_winRect.bottom = winRect.bottom;
}


const InputEventsCollection& InputModule::GetEvents() const
{
	return _events;
}

bool InputModule::IsKeyDown(EKeyCode key) const
{
	return _keysDownMap[key];
}

bool InputModule::IsKeyUp(EKeyCode key) const
{
	return _keysDownMap[key];
}

bool InputModule::IsMouseMoved() const
{
	return _mouseX != _mousePrevX || _mouseY != _mousePrevY;
}
int InputModule::GetMouseDeltaX() const
{
	return _mouseX - _mousePrevX;
}
int InputModule::GetMouseDeltaY() const
{
	return _mouseY - _mousePrevY;
}


void InputModule::DecodeKey(const UINT& msg, WPARAM wParam, EKeyCode& outKey, bool& outIsDown)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		outKey = static_cast<EKeyCode>(wParam);
		outIsDown = true;
		break;
	case WM_KEYUP:
		outKey = static_cast<EKeyCode>(wParam);
		outIsDown = false;
		break;
	case WM_LBUTTONDOWN:
		outKey = MOUSE_LEFT;
		outIsDown = true;
		break;
	case WM_LBUTTONUP:
		outKey = MOUSE_LEFT;
		outIsDown = false;
		break;
	case WM_RBUTTONDOWN:
		outKey = MOUSE_RIGHT;
		outIsDown = true;
		break;
	case WM_RBUTTONUP:
		outKey = MOUSE_RIGHT;
		outIsDown = false;
	case WM_MBUTTONDOWN:
		outKey = MOUSE_MIDDLE;
		outIsDown = true;
		break;
	case WM_MBUTTONUP:
		outKey = MOUSE_MIDDLE;
		outIsDown = false;
		break;
	}
}
