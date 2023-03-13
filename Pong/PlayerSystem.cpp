#include "PlayerSystem.h"

#include "GeometryRenderBuilder.h"

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

	_ePlayer = _ecs->CreateEntity();

	auto& tf = _ecs->AddComp<TransformComp>(_ePlayer);
	tf.Pos = startPos;
	tf.Scale = _platformScale;

	_ecs->AddComp<BoxCollisionComp>(_ePlayer);
	auto& renderComp = _ecs->AddComp<RenderComp>(_ePlayer);
	GeometryRenderBuilder::BuildSquare(renderComp, Color::White());
	renderComp.DrawerId = 0;
	renderComp.ShaderId = 0;
}

void PlayerSystem::Run()
{
	float deltaTime = _engine->Time()->GetFrameDeltaSecs();

	if (_engine->Input()->IsKeyDown(_keyMoveUp))
	{
		auto& tf = _ecs->GetComp<TransformComp>(_ePlayer);
		float delta = _moveSpeed * deltaTime;
		if (tf.Pos.Y + delta > _borderPosY)
			tf.Pos.Y = _borderPosY;
		else
			tf.Pos.Y += delta;
	}
	if (_engine->Input()->IsKeyDown(_keyMoveDown))
	{
		auto& tf = _ecs->GetComp<TransformComp>(_ePlayer);
		float delta = _moveSpeed * deltaTime;
		if (tf.Pos.Y - delta < -_borderPosY)
			tf.Pos.Y = -_borderPosY;
		else
			tf.Pos.Y -= delta;
	}
}
