#pragma once

#include "Vector3.h"

namespace BlahEngine
{
struct CollisionComp;
struct TransformComp;

class PhysicsModule
{
public:
	PhysicsModule() = default;
	~PhysicsModule();
	
	void Init(entt::registry* ecs);

	void AdjustChildrenTransforms();

	void RunCollisions();

private:
	entt::registry* _ecs;

	Vector3 GetBoxBoxIntersectDir(
		const Vector3& posA, const Vector3& sizeA,
		const Vector3& posB, const Vector3 sizeB);
};
}
