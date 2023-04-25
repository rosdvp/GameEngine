#pragma once
#include "Vector3.h"
#include "DirectXCollision.h"

namespace BlahEngine
{
struct CollisionComp
{
	enum ECollisionType
	{
		BoxCollision,
		SphereCollision
	};

	bool IsStatic{ false };
	ECollisionType Type{ BoxCollision };
	Vector3 Size{ 1, 1, 1 };

	DirectX::BoundingOrientedBox Box{};
	DirectX::BoundingSphere Sphere{};
	
	std::vector<entt::entity> Collided;
};
}
