#pragma once

#include "BlahEngine.h"

class LevelSystem : public BlahEngine::IInitSystem
{
public:
	void Init() override;

private:
	void CreateLight();

	void CreateGround();

	void CreateObstacle(BlahEngine::Vector3 pos);
};