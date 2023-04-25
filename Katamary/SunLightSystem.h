#pragma once

#include "BlahEngine.h"


class SunLightSystem : public BlahEngine::IInitSystem, BlahEngine::IRunSystem
{
public:
	void Init() override;

	void Run() override;

private:
	entt::entity _entLight{ entt::null };
};