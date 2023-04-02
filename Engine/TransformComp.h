#pragma once
#include "Vector3.h"
#include "entt.hpp"

namespace BlahEngine
{
struct TransformComp
{
public:
	Vector3 Pos{ 0, 0, 0 };
	Vector3 Rot{ 0, 0, 0 };
	Vector3 Scale{ 1, 1, 1 };

	Vector3 PrevPos{ 0, 0, 0 };
	Vector3 PrevRot{ 0, 0, 0 };
	Vector3 PrevScale{ 1, 1, 1 };

	Vector3 GlobalPos{ 0, 0, 0 };
	Vector3 PrevGlobalPos{ 0, 0, 0 };
	
	entt::entity Parent = entt::null;
	entt::entity PrevParent = entt::null;

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

	bool IsPosOrRotOrScaleChanged() const
	{
		return Pos != PrevPos || GlobalPos != PrevGlobalPos || Rot != PrevRot || Scale != PrevScale;
	}

private:
};
}

//template <>
//struct std::hash<BlahEngine::TransformComp>
//{
//	std::size_t operator()(const BlahEngine::TransformComp& t) const noexcept
//	{
//
//	}
//};