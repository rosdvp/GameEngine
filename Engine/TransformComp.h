#pragma once
#include "Vector3.h"
#include "entt.hpp"

namespace BlahEngine
{
struct TransformComp
{
public:
	Vector3 Pos { 0, 0, 0};
	Vector3 Rot { 0, 0, 0};
	Vector3 Scale{ 1, 1, 1 };

	Vector3 PrevPos { 0, 0, 0 };
	Vector3 PrevRot { 0, 0, 0 };
	Vector3 PrevScale { 1, 1, 1 };

	bool IsJustAdded;
	entt::entity Parent = entt::null;
	std::vector<entt::entity> Children;

	Vector3 GetForward() const
	{
		return Vector3{ 0, 0, 1 }.GetRotX(Rot.X).GetRotY(Rot.Y).GetRotZ(Rot.Z);
	}

	Vector3 GetRight() const
	{
		return Vector3{ 1, 0, 0 }.GetRotX(Rot.X).GetRotY(Rot.Y).GetRotZ(Rot.Z);
	}

	Vector3 GetUp() const
	{
		return Vector3{ 0, 1, 0 }.GetRotX(Rot.X).GetRotY(Rot.Y).GetRotZ(Rot.Z);
	}

	bool IsChanged() const
	{
		return Pos != PrevPos || Rot != PrevRot || Scale != PrevScale;
	}
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