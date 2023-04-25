#pragma once

struct PlayerComp
{
	int CollectedCount = 0;

	float BallScaleBase;
	float BallScaleStep;

	float CollectableScaleRatio;
};