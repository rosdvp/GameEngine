#include "pch.h"
#include "PhysicsModule.h"

#include "Logger.h"

using namespace BlahEngine;

PhysicsModule::~PhysicsModule()
{
	Logger::Debug("collision module", "destroyed");
}


void PhysicsModule::Init(EcsCore* ecs)
{
	_ecs = ecs;

	_collisionsFilter = ecs->GetFilter(FilterMask().Inc<TransformComp>().Inc<BoxCollisionComp>());

	Logger::Debug("collision module", "initialized");
}

bool PhysicsModule::IsBlockAny(Entity entity)
{
	PhysicsBlockData dummy;
	return IsBlockAny(entity, dummy);
}

bool PhysicsModule::IsBlockAny(Entity entity, PhysicsBlockData& outData)
{
	auto& tfA = _ecs->GetComp<TransformComp>(entity);
	auto& boxA = _ecs->GetComp<BoxCollisionComp>(entity);

	for (auto ent : *_collisionsFilter)
		if (ent != entity)
		{
			auto& tfB = _ecs->GetComp<TransformComp>(ent);
			auto& boxB = _ecs->GetComp<BoxCollisionComp>(ent);
			if (IsBoxBoxIntersected(tfA, boxA, tfB, boxB))
			{
				outData.Ent = ent;
				outData.DirToEnt = (tfA.Pos - tfB.Pos).Norm();
				return true;
			}
		}
	return false;
}

bool PhysicsModule::TryMove(Entity entity, const Vector3& deltaPos)
{
	auto& tf = _ecs->GetComp<TransformComp>(entity);
	auto prevPos = tf.Pos;
	tf.Pos += deltaPos;
	if (IsBlockAny(entity))
	{
		tf.Pos = prevPos;
		return false;
	}
	return true;
}

bool PhysicsModule::IsBoxBoxIntersected(
	const TransformComp& tfA, const BoxCollisionComp& boxA,
	const TransformComp& tfB, const BoxCollisionComp& boxB)
{
	Vector3 sizeA = tfA.Scale * boxA.Size;
	Vector3 sizeB = tfB.Scale * boxB.Size;

	return abs(tfA.Pos.X - tfB.Pos.X) < (sizeA.X + sizeB.X) &&
		abs(tfA.Pos.Y - tfB.Pos.Y) < (sizeA.Y + sizeB.Y) &&
		abs(tfA.Pos.Z - tfB.Pos.Z) < (sizeA.Z + sizeB.Z);
}

Vector3 PhysicsModule::GetBoxBoxIntersectDir(
	const Vector3& posA, const Vector3& sizeA, 
	const Vector3& posB, const Vector3 sizeB)
{
	Vector3 points[8] = {
		{posB.X + sizeA.X / 2, posB.Y + sizeA.Y / 2, posB.Z + sizeA.Z / 2},
		{posB.X - sizeA.X / 2, posB.Y + sizeA.Y / 2, posB.Z + sizeA.Z / 2},
		{posB.X + sizeA.X / 2, posB.Y - sizeA.Y / 2, posB.Z + sizeA.Z / 2},
		{posB.X + sizeA.X / 2, posB.Y + sizeA.Y / 2, posB.Z - sizeA.Z / 2},
		{posB.X - sizeA.X / 2, posB.Y - sizeA.Y / 2, posB.Z + sizeA.Z / 2},
		{posB.X - sizeA.X / 2, posB.Y + sizeA.Y / 2, posB.Z - sizeA.Z / 2},
		{posB.X + sizeA.X / 2, posB.Y - sizeA.Y / 2, posB.Z - sizeA.Z / 2},
		{posB.X - sizeA.X / 2, posB.Y - sizeA.Y / 2, posB.Z - sizeA.Z / 2},
	};

	bool isIntersected[8];
	for (int i = 0; i < 8; i++)
	{
		isIntersected[i] = 
			posA.X - sizeA.X / 2 <= points[i].X && points[i].X <= posA.X + sizeA.X &&
			posA.Y - sizeA.Y / 2 <= points[i].Y && points[i].Y <= posA.Y + sizeA.Y &&
			posA.Z - sizeA.Z / 2 <= points[i].Z && points[i].Z <= posA.Z + sizeA.Z;
	}

	Vector3 avgDir {};
	int avgCount = 0;
	for (int i = 0; i < 8; i++)
		if (isIntersected[i])
		{
			avgDir += points[i] - posB;
			avgCount++;
		}
	avgDir /= avgCount;

	return avgDir;
}
