#pragma once
#include "BlahEngine.h"

class CollectSystem : public BlahEngine::IInitSystem, BlahEngine::IRunSystem
{
public:
	void Init() override;
	void Run() override;
};

