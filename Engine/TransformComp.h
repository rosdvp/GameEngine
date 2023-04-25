#pragma once
#include "Vector3.h"
#include "entt.hpp"
#include "Rotation.h"

namespace BlahEngine
{
struct TransformComp
{
public:
	Vector3 Pos{ 0, 0, 0 };
	Rotation Rot{ 0, 0, 0 };
	Vector3 Scale{ 1, 1, 1 };
	
	Vector3 PrevPos{ 0, 0, 0 };			//do not change, for physics purposes
	
	Vector3 GlobalPos{ 0, 0, 0 };				//do not change, calculated automatically
	Rotation GlobalRot{ 0, 0, 0 };		//do not change, calculated automatically

	entt::entity Parent = entt::null;
	entt::entity PrevParent = entt::null;		//do not change

	Vector3 GetForward() const
	{
		return Vector3{ 0, 0, 1 }.Rotate(GlobalRot.GetQuaternionRef());
	}

	Vector3 GetRight() const
	{
		return Vector3{ 1, 0, 0 }.Rotate(GlobalRot.GetQuaternionRef());
	}

	Vector3 GetUp() const
	{
		return Vector3{ 0, 1, 0 }.Rotate(GlobalRot.GetQuaternionRef());
	}

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
		HashCombine(h, Pos, Rot, Scale, GlobalPos, GlobalRot);
		return h;
	}
};
}