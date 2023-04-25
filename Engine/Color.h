#pragma once
#include <directxmath.h>

#include "Hasher.h"

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
		R(r), G(g), B(b), A(a)
	{
	}

	float R, G, B, A = 0.0f;

	operator DirectX::XMFLOAT4() const
	{
		return { R, G, B, A };
	}
};
}


template <>
struct std::hash<BlahEngine::Color>
{
	std::size_t operator()(const BlahEngine::Color& c) const noexcept
	{
		std::size_t h = 0;
		BlahEngine::HashCombine(h, c.R, c.G, c.B, c.A);
		return h;
	}
};