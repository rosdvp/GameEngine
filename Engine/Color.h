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
		//using namespace std;
		//
		//size_t h1 = hash<float>{}(c.R);
		//size_t h2 = hash<float>{}(c.G);
		//size_t h3 = hash<float>{}(c.B);
		//size_t h4 = hash<float>{}(c.A);
		//
		//size_t h = h1 ^ (h2 >> 1);
		//
		//return ((((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1)) >> 1) ^ (h4 << 1);

		std::size_t h = 0;
		BlahEngine::HashCombine(h, c.R, c.G, c.B, c.A);
		return h;
	}
};