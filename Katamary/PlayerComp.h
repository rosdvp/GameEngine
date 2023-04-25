#pragma once

struct PlayerComp
{
	BlahEngine::Vector3 Force {};

	int CollectedCount = 0;

	float BallScaleBase;
	float BallScaleStep;

	float CollectableScaleRatio;
};