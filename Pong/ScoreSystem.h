#pragma once
#include "BlahEngine.h"

class ScoreSystem : public BlahEngine::IInitSystem, public BlahEngine::IRunSystem
{
public:
	void Init() override;

	void Run() override;

private:
	entt::entity _ent{ entt::null };
	
	int _score0 = 0;
	int _score1 = 0;
};