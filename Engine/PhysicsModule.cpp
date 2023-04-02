#include "pch.h"
#include "PhysicsModule.h"

#include "Logger.h"
#include "CollisionComp.h"
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
	auto view = _ecs->view<TransformComp, CollisionComp>();

	view.each([](TransformComp& tf, CollisionComp& col)
		{
			col.Collided.clear();
			col.Box.Center = { tf.Pos.X, tf.Pos.Y, tf.Pos.Z };
			col.Box.Extents = { tf.Scale.X * col.Size.X, tf.Scale.Y * col.Size.Y, tf.Scale.Z * col.Size.Z};
		});

	for (auto entA : view)
	{
		auto [tfA, colA] = _ecs->get<TransformComp, CollisionComp>(entA);

		for (auto entB : view)
		{
			if (entA <= entB) //escape double check
				continue;

			auto [tfB, colB] = _ecs->get<TransformComp, CollisionComp>(entB);
			if (colA.Box.Intersects(colB.Box))
			{
				std::cout << "collided" << std::endl;
				colA.Collided.push_back(entB);
				colB.Collided.push_back(entA);

				if (!colA.IsStatic)
				{
					tfA.Pos = tfA.PrevPos;
				}
				else if (!colB.IsStatic)
				{
					tfB.Pos = tfB.PrevPos;
				}
			}
		}
	}
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
