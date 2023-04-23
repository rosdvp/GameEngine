#pragma once
#include "BlahEngine.h"

class PlayerCameraSystem : public BlahEngine::IInitSystem, BlahEngine::IRunSystem
{
public:
	void Init() override;
	void Run() override;

private:
	static constexpr float POS_OFFSET_Y = 1;
	static constexpr float SENSITIVITY_ROTATE = 50.0f;
	static constexpr float SENSITIVITY_ZOOM = 100.0f;


	entt::entity _entPlayer = entt::null;
	entt::entity _entCamera = entt::null;

	float _distToPlayer = 10;


	float ClampPitchDelta(float currPitch, float deltaPitch);
};