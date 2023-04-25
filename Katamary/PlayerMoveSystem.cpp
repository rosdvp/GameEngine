#include "PlayerMoveSystem.h"

#include "GeometryRenderBuilder.h"
#include "PlayerComp.h"
#include "RenderComp.h"
#include "RenderMaterialsPresets.h"

using namespace BlahEngine;

void PlayerMoveSystem::Init()
{
	_entPlayer = _ecs->create();

	auto& player = _ecs->emplace<PlayerComp>(_entPlayer);
	player.BallScaleBase = 1.0f;
	player.BallScaleStep = 0.1f;
	player.CollectableScaleRatio = 0.5f;

	auto& tf = _ecs->emplace<TransformComp>(_entPlayer);
	tf.Pos = { 0, 0.5f + player.BallScaleBase / 2.0f, 0 };
	tf.Scale = { player.BallScaleBase, player.BallScaleBase, player.BallScaleBase };

	auto& render = _ecs->emplace<RenderComp>(_entPlayer);
	render.ShaderId = RenderModule::EShaderId::SimpleLit;
	render.Mat = MatPresetChrome;
	GeometryRenderBuilder::BuildSphere(render, 20, Color::Pink());
	for (int i = 0; i < render.VerticesCount; i += 5)
		render.Vertices[i].Color = Color::Yellow();

	auto& col = _ecs->emplace<CollisionComp>(_entPlayer);
	col.Type = CollisionComp::SphereCollision;
	col.Size = { 1, 1, 1 };
	col.IsStatic = false;
}

void PlayerMoveSystem::Run()
{
	if (_entCamera == entt::null)
	{
		auto view = _ecs->view<const RenderCameraComp>();
		if (!view.empty())
			_entCamera = view.front();
		return;
	}

	float deltaTime = _engine->Time().GetFrameDeltaSecs();

	auto& playerTf = _ecs->get<TransformComp>(_entPlayer);
	auto& cameraTf = _ecs->get<TransformComp>(_entCamera);

	auto forward = cameraTf.GetForward();
	forward.Y = 0;
	auto right = cameraTf.GetRight();
	right.Y = 0;

	Vector3 dir;

	if (_engine->Input().IsKeyDown(KEY_W))
		dir += forward;
	if (_engine->Input().IsKeyDown(KEY_S))
		dir -= forward;
	if (_engine->Input().IsKeyDown(KEY_A))
		dir -= right;
	if (_engine->Input().IsKeyDown(KEY_D))
		dir += right;
	dir = dir.GetNorm();

	_force += dir * MOVE_ACCEL * deltaTime;

	playerTf.Pos += _force * deltaTime;

	auto deltaRot = _force * ROT_K / playerTf.Scale.X;

	playerTf.Rot.AddAroundWorld(-deltaRot.X, deltaRot.Z, 0);
}