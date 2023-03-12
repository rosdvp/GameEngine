#pragma once
#include "pch.h"

struct UiTextComp
{
	std::wstring Text;

	std::wstring Font;
	float FontSize;
	bool IsBold;

	ComPtr<IDWriteTextFormat> TextFormat;


	void Release()
	{
		IsBold = false;
		TextFormat.Reset();
	}
};
