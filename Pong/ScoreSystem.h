#pragma once
#include "BlahEngine.h"

class ScoreSystem : public BlahEngine::IInitSystem, public BlahEngine::IRunSystem
{
public:
	void Init() override;

	void Run() override;

private:
	BlahEngine::Entity _entity {};

	BlahEngine::Filter* _scoreIncreaseCmdFilter = nullptr;

	int _score0 = 0;
	int _score1 = 0;
};