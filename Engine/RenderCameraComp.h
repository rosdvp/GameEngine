#pragma once
#include "pch.h"


namespace BlahEngine
{
struct RenderCameraComp
{
	bool IsOrthographic;
	float OrthoZoom = 1.0f;
	float PerspectiveAngle = 1.0f;
	
	ComPtr<ID3D11Buffer> ConstantBuffer;

	bool IsChanged() const
	{
		return _lastHash != GetHash();
	}

	void ResetIsChanged()
	{
		_lastHash = GetHash();
	}

	size_t GetHash() const
	{
		std::size_t h1 = std::hash<bool>{}(IsOrthographic);
		std::size_t h2 = std::hash<float>{}(OrthoZoom);
		return h1 ^ (h2 << 1);
	}


	void Release()
	{
		ConstantBuffer.Reset();
	}

private:
	size_t _lastHash = 0;
};
}
