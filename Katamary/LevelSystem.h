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

	void CreateObstacle(BlahEngine::Vector3 pos, std::string pathModelFile, std::wstring pathTextureFile, float scaleFactor);
};