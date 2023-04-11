#pragma once
#include "pch.h"

#include "Vector3.h"


namespace BlahEngine
{
struct RenderCameraComp
{
	bool IsOrthographic = false;
	float OrthoZoom = 1.0f;
	float PerspectiveAngle = 1.0f;
	
	ComPtr<ID3D11Buffer> ViewAndProjConstantBuffer;

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
		HashCombine(h, IsOrthographic, OrthoZoom, PerspectiveAngle);
		return h;
	}
};
}