#pragma once

#include "Vector3.h"

namespace BlahEngine
{
struct BoxCollisionComp;
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

	bool IsBoxBoxIntersected(
		const TransformComp& tfA, const BoxCollisionComp& boxA,
		const TransformComp& tfB, const BoxCollisionComp& boxB);

	Vector3 GetBoxBoxIntersectDir(
		const Vector3& posA, const Vector3& sizeA,
		const Vector3& posB, const Vector3 sizeB);
};
}
