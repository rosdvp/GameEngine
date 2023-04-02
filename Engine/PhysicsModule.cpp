#include "pch.h"
#include "PhysicsModule.h"

#include "Logger.h"
#include "BoxCollisionComp.h"
#include "TransformComp.h"


using namespace BlahEngine;

PhysicsModule::~PhysicsModule()
{
	Logger::Debug("collision module", "destroyed");
}


void PhysicsModule::Init(entt::registry* ecs)
{
	_ecs = ecs;
	
	Logger::Debug("collision module", "initialized");
}

void PhysicsModule::AdjustChildrenTransforms()
{
}

void PhysicsModule::RunCollisions()
{
	auto view = _ecs->view<TransformComp, BoxCollisionComp>();

	view.each([](TransformComp& tf, BoxCollisionComp& box)
		{
			box.Collided.clear();
		});

	for (auto entA : view)
	{
		auto [tfA, boxA] = _ecs->get<TransformComp, BoxCollisionComp>(entA);

		for (auto entB : view)
		{
			if (entA <= entB) //escape double check
				continue;

			auto [tfB, boxB] = _ecs->get<TransformComp, BoxCollisionComp>(entB);
			if (IsBoxBoxIntersected(tfA, boxA, tfB, boxB))
			{
				boxA.Collided.push_back(entB);
				boxB.Collided.push_back(entA);

				if (!boxA.IsStatic)
				{
					tfA.Pos = tfA.PrevPos;
				}
				else if (!boxB.IsStatic)
				{
					tfB.Pos = tfB.PrevPos;
				}
			}
		}
	}
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
