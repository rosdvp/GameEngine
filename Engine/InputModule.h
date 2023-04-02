#pragma once

#include "InputEvents.h"

namespace BlahEngine
{
class InputModule
{
public:
	InputModule();
	~InputModule();

	void Init();

	void Run();

	void AddEvent(const UINT& msg, WPARAM wParam, LPARAM lParam);
	void Clear();

	void SetWinRect(const RECT& winRect);
	
	bool IsKeyDown(EKeyCode key) const;
	bool IsKeyPressed(EKeyCode key) const;
	bool IsKeyReleased(EKeyCode key) const;

	bool IsMouseMoved() const;
	int GetMouseDeltaX() const;
	int GetMouseDeltaY() const;

private:
	RECT _winRect;
	
	int _mouseX;
	int _mouseY;
	int _mousePrevX;
	int _mousePrevY;

	int _isMouseWheelChanged;
	int _mouseWheel;

	std::unique_ptr<bool[]> _keysDownMap;
	std::unique_ptr<bool[]> _keysPressedMap;
	std::unique_ptr<bool[]> _keysReleasedMap;

	void DecodeKey(const UINT& msg, WPARAM wParam, EKeyCode& outKey, bool& outIsDown);
};
}