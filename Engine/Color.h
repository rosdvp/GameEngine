#pragma once
#include <directxmath.h>

namespace BlahEngine
{
struct Color
{
public:
	static Color White()
	{
		return { 1, 1, 1, 1 };
	}
	static Color Black()
	{
		return { 0, 0, 0, 1 };
	}
	static Color Grey()
	{
		return { 0.5f, 0.5f, 0.5f, 1 };
	}
	static Color Red()
	{
		return { 1, 0, 0, 1 };
	}
	static Color Green()
	{
		return { 0, 1, 0, 1 };
	}
	static Color Blue()
	{
		return { 0, 0, 1, 1 };
	}
	static Color Yellow()
	{
		return { 1, 1, 0, 1 };
	}
	static Color Purple()
	{
		return { 0.5f, 0, 1, 1 };
	}
	static Color Pink()
	{
		return { 1, 0, 1, 1 };
	}
	static Color Orange()
	{
		return { 1, 0.5f, 0, 1 };
	}


public:
	Color(float r, float g, float b, float a) :
		_color(r, g, b, a)
	{
	}

	operator DirectX::XMFLOAT4() const
	{
		return _color;
	}

private:
	DirectX::XMFLOAT4 _color;
};
}