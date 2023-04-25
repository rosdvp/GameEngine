#include "SunLightSystem.h"
#include "RenderLightComp.h"
#include "GeometryRenderBuilder.h"

using namespace BlahEngine;

void SunLightSystem::Init()
{
	_entLight = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(_entLight);
	tf.Rot.Set(0, 20, 0);
	tf.Pos = { 0, 0, -10 };

	auto& light = _ecs->emplace<RenderLightComp>(_entLight);
	light.Color = Color::White();
	light.AmbientIntensity = 1.0f;

	auto& render = _ecs->emplace<RenderComp>(_entLight);
	render.ShaderId = RenderModule::EShaderId::SimpleUnlit;
	GeometryRenderBuilder::BuildCube(render, Color::Green());

	for (int i = 0; i < 4; i++)
		render.Vertices[i + 16].Color = Color::Red();
	for (int i = 0; i < 4; i++)
		render.Vertices[i + 20].Color = Color::Blue();
}

void SunLightSystem::Run() {
	float deltaTime = _engine->Time().GetFrameDeltaSecs();

	auto& tf = _ecs->get<TransformComp>(_entLight);
	auto& light = _ecs->get<RenderLightComp>(_entLight);

	float moveSpeed = 1 * deltaTime;
	float rotSpeed = 40 * deltaTime;

	if (_engine->Input().IsKeyDown(KEY_UP))
		tf.Pos += tf.GetForward() * moveSpeed;
	if (_engine->Input().IsKeyDown(KEY_DOWN))
		tf.Pos -= tf.GetForward() * moveSpeed;
	if (_engine->Input().IsKeyDown(KEY_RIGHT))
		tf.Pos += tf.GetRight() * moveSpeed;
	if (_engine->Input().IsKeyDown(KEY_LEFT))
		tf.Pos -= tf.GetRight() * moveSpeed;

	if (_engine->Input().IsKeyDown(KEY_I))
		tf.Rot.AddAroundLocal(0, rotSpeed, 0);
	if (_engine->Input().IsKeyDown(KEY_K))
		tf.Rot.AddAroundLocal(0, -rotSpeed, 0);
	if (_engine->Input().IsKeyDown(KEY_L))
		tf.Rot.AddAroundWorld(0, 0, rotSpeed);
	if (_engine->Input().IsKeyDown(KEY_J))
		tf.Rot.AddAroundWorld(0, 0, -rotSpeed);

	if (_engine->Input().IsKeyPressed(KEY_T))
		_engine->Render().DebugSetShadowCasterView(true);
	if (_engine->Input().IsKeyPressed(KEY_G))
		_engine->Render().DebugSetShadowCasterView(false);

	tf.Pos = Vector3{ 0, 0, -1 }.Rotate(tf.Rot.GetQuaternion()) * 30;
}
