#pragma once

#include "BlahEngine.h"

class FreeCameraSystem : public BlahEngine::IInitSystem, public BlahEngine::IRunSystem
{
public:
	void Init() override;
	void Run() override;

private:
	const float _moveSpeed = 5.0f;

	entt::entity _entCamera{ entt::null };

	void SetDefaultPos(BlahEngine::TransformComp& tf);
};