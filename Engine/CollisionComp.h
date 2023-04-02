#pragma once
#include "Vector3.h"
#include "DirectXCollision.h"

namespace BlahEngine
{
struct CollisionComp
{
	bool IsStatic = false;

	Vector3 Size = { 1, 1, 1 };

	DirectX::BoundingBox Box{};
	
	std::vector<entt::entity> Collided;
};
}
