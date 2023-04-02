#include "BallSystem.h"

#include "CollisionComp.h"
#include "GeometryRenderBuilder.h"
#include "ScoreIncreaseCmd.h"

using namespace BlahEngine;

BallSystem::BallSystem(
	float borderPosX, float borderPosY, 
	Vector3 ballScale, 
	float moveBaseSpeed, float moveSpeedIncK) :
	_borderPosX(borderPosX),
	_borderPosY(borderPosY),
	_ballScale(ballScale),
	_moveBaseSpeed(moveBaseSpeed),
	_moveSpeedIncK(moveSpeedIncK),
	_moveSpeed(moveBaseSpeed)
{
}

void BallSystem::Init()
{
	_ent = _ecs->create();

	_ecs->emplace<TransformComp>(_ent).Scale = _ballScale;
	_ecs->emplace<CollisionComp>(_ent).IsStatic;

	auto& render = _ecs->emplace<RenderComp>(_ent);
	GeometryRenderBuilder::BuildSquare(render, Color::White());
	render.DrawerId = 0;
	render.ShaderId = 0;

	_dir = { -1, -1, 0 };
}

void BallSystem::Run()
{
	auto& tf = _ecs->get<TransformComp>(_ent);
	auto& box = _ecs->get<CollisionComp>(_ent);

	if (tf.Pos.Y < -_borderPosY)
	{
		_dir.Y = -_dir.Y;
		tf.Pos.Y = -_borderPosY;
		return;
	}
	if (tf.Pos.Y > _borderPosY)
	{
		_dir.Y = -_dir.Y;
		tf.Pos.Y = _borderPosY;
		return;
	}

	if (box.Collided.size() > 0)
	{
		auto collidedEnt = box.Collided[0];
		auto collidedTf = _ecs->get<TransformComp>(collidedEnt);

		_dir = tf.Pos - collidedTf.Pos;
		_dir.X = _dir.X < 0 ? -1 : 1;
		_dir.Y = _dir.Y < 0 ? -1 : 1;
		_dir.Z = 0;
		return;
	}

	bool isPlayer0Goal = tf.Pos.X > _borderPosX;
	bool isPlayer1Goal = tf.Pos.X < -_borderPosX;

	if (isPlayer0Goal || isPlayer1Goal)
	{
		auto scoreCmd = _ecs->create();
		_ecs->emplace<ScoreIncreaseCmd>(scoreCmd).PlayerId = isPlayer0Goal ? 0 : 1;

		_dir = -_dir;
		_moveSpeed = _moveBaseSpeed;
		tf.Pos = { 0, 0, 0 };
		return;
	}

	tf.Pos += _dir * _moveSpeed * _engine->Time().GetFrameDeltaSecs();
}
