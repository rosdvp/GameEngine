#pragma once
#include "BlahEngine.h"

class PlayerMoveSystem : public BlahEngine::IInitSystem, BlahEngine::IRunSystem
{
public:
	void Init() override;
	void Run() override;

private:
	static constexpr float MOVE_ACCEL = 5.0f;
	static constexpr float ROT_K = 1.0f;

	entt::entity _entPlayer = entt::null;
	entt::entity _entCamera = entt::null;

	BlahEngine::Vector3 _force;
};