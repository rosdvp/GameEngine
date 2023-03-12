#pragma once
#include "InputCodes.h"

namespace BlahEngine
{
struct InputEvent
{
	EKeyCode Key;
	bool IsDown;
	wchar_t Symbol;
};

class InputEventsCollection
{
public:
	InputEventsCollection(int maxCount) :
		_maxCount(maxCount)
	{
		_events = new InputEvent[maxCount];
		_count  = 0;
	}

	~InputEventsCollection()
	{
		delete[] _events;
	}

	void Clear()
	{
		_count = 0;
	}

	void TryAdd(EKeyCode key, bool isDown, wchar_t symbol)
	{
		auto event    = _count == _maxCount ? (_events + _count - 1) : (_events + (_count++));
		event->Key    = key;
		event->IsDown = isDown;
		event->Symbol = symbol;
	}

	const InputEvent* begin() const { return &_events[0]; }
	const InputEvent* end() const { return &_events[_count]; }

private:
	const int _maxCount;

	InputEvent* _events;
	int _count;
};
}