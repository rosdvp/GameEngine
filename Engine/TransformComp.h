#pragma once
#include "Vector3.h"

namespace BlahEngine
{
struct TransformComp
{
public:
	Vector3 Pos { 0, 0, 0};
	Vector3 Rot { 0, 0, 0};
	Vector3 Scale{ 1, 1, 1 };


	void Release()
	{
		Pos = { 0, 0, 0 };
		Rot = { 0, 0, 0 };
		Scale = { 1, 1, 1 };
	}


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
		std::size_t h1 = std::hash<Vector3>{}(Pos);
		std::size_t h2 = std::hash<Vector3>{}(Rot);
		std::size_t h3 = std::hash<Vector3>{}(Scale);
		return (h1 ^ (h2 << 1) >> 1) ^ (h3 << 1);
	}

private:
	size_t _lastHash = 0;
};
}
//
// template <>
// struct std::hash<BlahEngine::TransformComp>
// {
// 	std::size_t operator()(const BlahEngine::TransformComp& t) const noexcept
// 	{
// 		return t.GetHash();
// 	}
// };