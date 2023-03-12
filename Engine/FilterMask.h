#pragma once
#include <functional>
#include "CompPool.h"

namespace BlahEngine
{
struct FilterMask
{
	CompId IncIds;
	CompId ExcIds;

	bool operator==(const FilterMask& other) const
	{
		return IncIds == other.IncIds && ExcIds == other.ExcIds;
	}

	template<typename T>
	FilterMask& Inc()
	{
		IncIds |= CompPool<T>::GetId();
		return *this;
	}

	template<typename T>
	FilterMask& Dec()
	{
		ExcIds |= CompPool<T>::GetId();
		return *this;
	}
};
}

template <>
struct std::hash<BlahEngine::FilterMask>
{
	std::size_t operator()(const BlahEngine::FilterMask& mask) const noexcept
	{
		std::size_t h1 = std::hash<BlahEngine::CompId>{}(mask.IncIds);
		std::size_t h2 = std::hash<BlahEngine::CompId>{}(mask.ExcIds);
		return h1 ^ (h2 << 1);
	}
};