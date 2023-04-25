#pragma once

#include "BlahEngine.h"

class LevelSystem : public BlahEngine::IInitSystem, BlahEngine::IRunSystem
{
public:
	void Init() override;

	void Run() override;

private:
	entt::entity _entDebugBox {entt::null};

	void CreateGround();

	entt::entity CreateObstacle(BlahEngine::Vector3 pos, BlahEngine::Vector3 scale);

	void CreateDuck(const BlahEngine::Vector3& pos, const BlahEngine::Rotation& rot, float scale);
};