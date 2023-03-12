#include "PlayerSystem.h"

#include "SquareRenderBuilder.h"

using namespace BlahEngine;

PlayerSystem::PlayerSystem(int playerId,
	float moveSpeed,
	float posX, float borderPosY, BlahEngine::Vector3 platformScale) :
	_moveSpeed(moveSpeed),
	_borderPosY(borderPosY),
	_posX(posX),
	_platformScale(platformScale)
{
	_keyMoveUp = playerId == 0 ? KEY_W : KEY_UP;
	_keyMoveDown = playerId == 0 ? KEY_S : KEY_DOWN;
}

void PlayerSystem::Init()
{
	Vector3 startPos{ _posX, 0, 0};

	_player = _ecs->CreateEntity();

	auto& tf = _ecs->AddComp<TransformComp>(_player);
	tf.Pos = startPos;
	tf.Scale = _platformScale;

	_ecs->AddComp<BoxCollisionComp>(_player);
	SquareRenderBuilder::Build(_ecs->AddComp<RenderComp>(_player));
}

void PlayerSystem::Run()
{
	float deltaTime = _engine->Time()->GetFrameDeltaSecs();

	if (_engine->Input()->IsKeyDown(_keyMoveUp))
	{
		auto& tf = _ecs->GetComp<TransformComp>(_player);
		float delta = _moveSpeed * deltaTime;
		if (tf.Pos.Y + delta > _borderPosY)
			tf.Pos.Y = _borderPosY;
		else
			tf.Pos.Y += delta;
	}
	if (_engine->Input()->IsKeyDown(_keyMoveDown))
	{
		auto& tf = _ecs->GetComp<TransformComp>(_player);
		float delta = _moveSpeed * deltaTime;
		if (tf.Pos.Y - delta < -_borderPosY)
			tf.Pos.Y = -_borderPosY;
		else
			tf.Pos.Y -= delta;
	}
}
