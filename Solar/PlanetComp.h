#pragma once
#include "Entity.h"

struct PlanetComp
{
	bool IsRootBounded;
	BlahEngine::Entity Root;
	BlahEngine::Vector3 Dir;
	float OrbitRadius;
	float OrbitSpeed;
	float SelfRotateSpeed;
};
