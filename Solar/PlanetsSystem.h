#pragma once
#include "BlahEngine.h"
#include "Color.h"

class PlanetsSystem : public BlahEngine::IInitSystem, public BlahEngine::IRunSystem
{
public:
	void Init() override;
	void Run() override;

private:
	BlahEngine::Filter* _planetsFilter = nullptr;

	BlahEngine::Entity CreatePlanet(
		bool isRootBounded, BlahEngine::Entity root, 
		const BlahEngine::Vector3& initDir, float orbitRadius, float orbitSpeed, 
		float selfRotateSpeed, const BlahEngine::Vector3& scale, const BlahEngine::Color& color);
};
