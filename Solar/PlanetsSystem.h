#pragma once
#include "BlahEngine.h"
#include "Color.h"

class PlanetsSystem : public BlahEngine::IInitSystem, public BlahEngine::IRunSystem
{
public:
	void Init() override;
	void Run() override;

private:
	entt::entity CreatePlanet(
		entt::entity entParent,
		float orbitRadius, float orbitSpeed,
		float selfRotateSpeed, const BlahEngine::Vector3& scale, const BlahEngine::Color& color);
};
