#include "CollectSystem.h"

#include "PlayerComp.h"

using namespace BlahEngine;

void CollectSystem::Init() {}

void CollectSystem::Run()
{
	auto playerView = _ecs->view<PlayerComp, TransformComp, CollisionComp>();

	for (auto entPlayer : playerView)
	{
		auto [player, playerTf, playerCol] = playerView.get(entPlayer);

		if (playerCol.Collided.size() == 0)
			return;

		float maxScaleLength = playerTf.Scale.GetLength() * player.CollectableScaleRatio;

		for (auto entCollided : playerCol.Collided)
		{
			auto& objTf = _ecs->get<TransformComp>(entCollided);

			if (objTf.Scale.GetLength() > maxScaleLength)
				continue;

			objTf.Parent = entPlayer;
			_ecs->remove<CollisionComp>(entCollided);

			player.CollectedCount += 1;

			float newScale = player.BallScaleBase + player.BallScaleStep * player.CollectedCount;
			playerTf.Scale = { newScale, newScale, newScale };
			playerTf.Pos.Y = 0.5f + newScale / 2.0f;
		}
	}
}