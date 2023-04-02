#pragma once

#include "BlahEngine.h"

class BallSystem : public BlahEngine::IInitSystem, public BlahEngine::IRunSystem
{
public:
	BallSystem(
		float borderPosX, float borderPosY,
		BlahEngine::Vector3 ballScale,
		float moveBaseSpeed, float moveSpeedIncK);

	void Init() override;
	void Run() override;

private:
	const float _borderPosX;
	const float _borderPosY;
	const BlahEngine::Vector3 _ballScale;
	const float _moveBaseSpeed;
	const float _moveSpeedIncK;

	float _moveSpeed;
	
	entt::entity _ent{ entt::null };
	BlahEngine::Vector3 _dir {};
};