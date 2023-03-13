#pragma once

#include "BlahEngine.h"

class CameraSystem : public BlahEngine::IInitSystem, public BlahEngine::IRunSystem
{
public:
	CameraSystem(float moveSpeed);

	void Init() override;
	void Run() override;

private:
	const float _moveSpeed;

	BlahEngine::Entity _eCamera {};
};