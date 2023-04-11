#pragma once
#include "Color.h"

namespace BlahEngine
{
struct RenderLightComp
{
public:
	Color Color {0, 0, 0, 0};

	float AmbientIntensity = 0;

	bool IsChanged() const
	{
		return _lastHash != GetHash();
	}

	void ResetIsChanged()
	{
		_lastHash = GetHash();
	}

private:
	std::size_t _lastHash = 0;

	std::size_t GetHash() const
	{
		std::size_t h = 0;
		HashCombine(h, Color, AmbientIntensity);
		return h;
	}
};
}
