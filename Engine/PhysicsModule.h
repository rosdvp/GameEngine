#pragma once

#include "EcsCore.h"
#include "TransformComp.h"
#include "BoxCollisionComp.h"

namespace BlahEngine
{
struct PhysicsBlockData;

class IPhysicsModule
{
public:
	virtual ~IPhysicsModule() = default;

	virtual bool IsBlockAny(Entity entity) = 0;
	virtual bool IsBlockAny(Entity entity, PhysicsBlockData& outData) = 0;

	virtual bool TryMove(Entity entity, const Vector3& deltaPos) = 0;
};

class PhysicsModule : virtual public IPhysicsModule
{
public:
	PhysicsModule() = default;
	~PhysicsModule() override;

	void Init(EcsCore* ecs);

	bool IsBlockAny(Entity entity) override;
	bool IsBlockAny(Entity entity, PhysicsBlockData& outData) override;

	bool TryMove(Entity entity, const Vector3& deltaPos) override;

private:
	EcsCore* _ecs;
	Filter* _collisionsFilter;

	bool IsBoxBoxIntersected(
		const TransformComp& tfA, const BoxCollisionComp& boxA,
		const TransformComp& tfB, const BoxCollisionComp& boxB);

	Vector3 GetBoxBoxIntersectDir(
		const Vector3& posA, const Vector3& sizeA,
		const Vector3& posB, const Vector3 sizeB);
};

struct PhysicsBlockData
{
	Entity Ent;
	Vector3 DirToEnt;
};
}
