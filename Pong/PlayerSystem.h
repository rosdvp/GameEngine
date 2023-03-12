#pragma once
#include "BlahEngine.h"

class PlayerSystem : public BlahEngine::IInitSystem, public BlahEngine::IRunSystem
{
public:
	PlayerSystem(int playerId, 
		float moveSpeed, 
		float posX, float borderPosY, BlahEngine::Vector3 platformScale);

	void Init() override;
	void Run() override;

private:
	const float _moveSpeed;
	const float _borderPosY;
	const float _posX;
	const BlahEngine::Vector3 _platformScale { 0.3f, 1.5f, 1};

	BlahEngine::Entity _player {};

	BlahEngine::EKeyCode _keyMoveUp;
	BlahEngine::EKeyCode _keyMoveDown;
};