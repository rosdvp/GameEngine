#pragma once
#include "Vector3.h"

namespace BlahEngine
{
struct BoxCollisionComp
{
	bool IsStatic = false;

	Vector3 Size = { 1, 1, 1 };
	
	std::vector<entt::entity> Collided;
};
}
