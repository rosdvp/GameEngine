#pragma once
#include "BlahEngine.h"

class PlayerCameraSystem : public BlahEngine::IInitSystem, BlahEngine::IRunSystem
{
public:
	void Init() override;
	void Run() override;

private:
	static constexpr float DIST_TO_PLAYER = 10;
	static constexpr float POS_OFFSET_Y = 1;
	static constexpr float SENSITIVITY = 50.0f;


	entt::entity _entPlayer = entt::null;
	entt::entity _entCamera = entt::null;


};