#include "pch.h"
#include "InputModule.h"

#include "Logger.h"

using namespace BlahEngine;

InputModule::InputModule() :
	_winRect(),
	_mouseX(0),
	_mouseY(0),
	_mousePrevX(0),
	_mousePrevY(0),
	_isMouseWheelChanged(0),
	_mouseWheel(0) {}

InputModule::~InputModule()
{
	Logger::Debug("input module", "destroyed");
}

void InputModule::Init()
{
	_keysDownMap = std::make_unique<bool[]>(KEY_MAX);
	_keysPressedMap = std::make_unique<bool[]>(KEY_MAX);
	_keysReleasedMap = std::make_unique<bool[]>(KEY_MAX);
	for (int i = 0; i < KEY_MAX; i++)
	{
		_keysDownMap[i] = false;
		_keysPressedMap[i] = false;
		_keysReleasedMap[i] = false;
	}
	
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


bool InputModule::IsKeyDown(EKeyCode key) const
{
	return _keysDownMap[key];
}
bool InputModule::IsKeyPressed(EKeyCode key) const
{
	return _keysPressedMap[key];
}
bool InputModule::IsKeyReleased(EKeyCode key) const
{
	return _keysReleasedMap[key];
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




void InputModule::AddEvent(const UINT& msg, WPARAM wParam, LPARAM lParam)
{
	EKeyCode key;
	bool isDown;
	wchar_t buffer[1]{ ' ' };

	DecodeKey(msg, wParam, key, isDown);

	if (isDown)
	{
		if (!_keysDownMap[key])
		{
			_keysPressedMap[key] = true;
			_keysDownMap[key] = true;
		}
	}
	else
	{
		if (_keysDownMap[key])
		{
			_keysReleasedMap[key] = true;
			_keysDownMap[key] = false;
		}
	}
	
	/*
	if (msg == WM_KEYDOWN || msg == WM_KEYUP)
	{
		BYTE keyState[256];
		GetKeyboardState(keyState);
		ToUnicode(wParam, HIWORD(lParam) & 0xFF, keyState, buffer, 1, 0);
	}

	_isMouseWheelChanged = true;
	_mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
	*/
}

void InputModule::Clear()
{
	_mousePrevX = _mouseX;
	_mousePrevY = _mouseY;

	_isMouseWheelChanged = false;

	for (int i = 0; i < KEY_MAX; i++)
	{
		_keysPressedMap[i] = false;
		_keysReleasedMap[i] = false;
	}
}

void InputModule::SetWinRect(const RECT& winRect)
{
	_winRect.left = winRect.left;
	_winRect.right = winRect.right;
	_winRect.top = winRect.top;
	_winRect.bottom = winRect.bottom;
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
