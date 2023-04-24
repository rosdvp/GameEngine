#pragma once

#include "BlahEngine.h"

class LevelSystem : public BlahEngine::IInitSystem
{
public:
	void Init() override;

private:
	void CreateLight();

	void CreateGround();

	void CreateObstacle(BlahEngine::Vector3 pos, BlahEngine::Vector3 scale);

	void CreateDuck(const BlahEngine::Vector3& pos, const BlahEngine::Rotation& rot, float scale);
};