#include "PlayerSystem.h"

#include "GeometryRenderBuilder.h"

using namespace BlahEngine;

PlayerSystem::PlayerSystem(int playerId,
	float moveSpeed,
	float posX, float borderPosY, BlahEngine::Vector3 platformScale) :
	_moveSpeed(moveSpeed),
	_borderPosY(borderPosY),
	_posX(posX),
	_platformScale(platformScale),
	_entPlayer(entt::null)
{
	_keyMoveUp   = playerId == 0 ? KEY_W : KEY_UP;
	_keyMoveDown = playerId == 0 ? KEY_S : KEY_DOWN;
}

void PlayerSystem::Init()
{
	Vector3 startPos{ _posX, 0, 0};

	_entPlayer = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(_entPlayer);
	tf.Pos = startPos;
	tf.Scale = _platformScale;

	_ecs->emplace<BoxCollisionComp>(_entPlayer).IsStatic = true;

	auto& render = _ecs->emplace<RenderComp>(_entPlayer);
	GeometryRenderBuilder::BuildSquare(render, Color::White());
	render.DrawerId = 0;
	render.ShaderId = 0;
}

void PlayerSystem::Run()
{
	float deltaTime = _engine->Time().GetFrameDeltaSecs();

	if (_engine->Input().IsKeyDown(_keyMoveUp))
	{
		auto& tf = _ecs->get<TransformComp>(_entPlayer);
		float delta = _moveSpeed * deltaTime;
		if (tf.Pos.Y + delta > _borderPosY)
			tf.Pos.Y = _borderPosY;
		else
			tf.Pos.Y += delta;
	}
	if (_engine->Input().IsKeyDown(_keyMoveDown))
	{
		auto& tf = _ecs->get<TransformComp>(_entPlayer);
		float delta = _moveSpeed * deltaTime;
		if (tf.Pos.Y - delta < -_borderPosY)
			tf.Pos.Y = -_borderPosY;
		else
			tf.Pos.Y -= delta;
	}
}
