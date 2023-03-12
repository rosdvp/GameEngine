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

	void AddEvent(const UINT& msg, WPARAM wParam, LPARAM lParam);
	void ClearEvents();

	void SetWinRect(const RECT& winRect);

	const InputEventsCollection& GetEvents() const;

	bool IsKeyDown(EKeyCode key) const;
	bool IsKeyUp(EKeyCode key) const;

private:
	RECT _winRect;

	int _mouseX;
	int _mouseY;

	int _isMouseWheelChanged;
	int _mouseWheel;

	InputEventsCollection _events;

	bool* _keysDownMap;

	void DecodeKey(const UINT& msg, WPARAM wParam, EKeyCode& outKey, bool& outIsDown);
};
}