#pragma once

namespace BlahEngine
{
struct Entity
{
	int Idx;

	bool operator==(const Entity& other) const
	{
		return Idx == other.Idx;
	}
	bool operator!=(const Entity& other) const
	{
		return Idx != other.Idx;
	}
};
}

template <>
struct std::hash<BlahEngine::Entity>
{
	std::size_t operator()(const BlahEngine::Entity& entity) const noexcept
	{
		return entity.Idx;
	}
};