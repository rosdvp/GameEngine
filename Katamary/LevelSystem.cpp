#include "LevelSystem.h"

#include "GeometryRenderBuilder.h"
#include "RenderComp.h"
#include "RenderLightComp.h"

using namespace BlahEngine;

void LevelSystem::Init()
{
	CreateLight();
	CreateGround();
	CreateObstacle({ 5, 1.0f, 5 }, {1, 1, 1});
	CreateObstacle({ 5, 1.5f, -5 }, {1, 2, 1});
	CreateObstacle({ -5, 2.0f, -5 }, {1, 3, 1});
	CreateObstacle({ -5, 2.5f, 5 }, {1, 4, 1});

	CreateDuck({ 0, 0.5f, 5 }, { 0, 90, 0 }, 0.15f);
}

void LevelSystem::CreateLight()
{
	auto ent = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Rot.Set(0, 0, 0);
	tf.Pos = { 0, 0, -10 };

	auto& light = _ecs->emplace<RenderLightComp>(ent);
	light.Color = Color::White();
	light.AmbientIntensity = 2.0f;

	auto& render = _ecs->emplace<RenderComp>(ent);
	render.ShaderId = RenderModule::EShaderId::SimpleUnlit;
	GeometryRenderBuilder::BuildCube(render, Color::Green());

	for (int i = 0; i < 4; i++)
		render.Vertices[i + 16].Color = Color::Red();
	for (int i = 0; i < 4; i++)
		render.Vertices[i + 20].Color = Color::Blue();
}

void LevelSystem::CreateGround()
{
	auto ent = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Scale = { 100, 1, 100 };

	auto& render = _ecs->emplace<RenderComp>(ent);
	render.ShaderId = RenderModule::EShaderId::SimpleLit;
	render.Mat = { 0.5f, 0.1f, 0.5f, 30 };
	GeometryRenderBuilder::BuildCube(render, Color::Grey());
}

void LevelSystem::CreateObstacle(Vector3 pos, Vector3 scale)
{
	auto ent = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Pos = pos;
	tf.Scale = scale;

	auto& render = _ecs->emplace<RenderComp>(ent);
	render.ShaderId = RenderModule::EShaderId::SimpleLit;
	render.Mat = { 0.5f, 0.1f, 0.5f, 30 };
	//render.Mat = { 0.2775f, 0.23125f, 0.773911f, 89.6 };

	GeometryRenderBuilder::BuildCube(render, Color::Green());
}

void LevelSystem::CreateDuck(const Vector3& pos, const Rotation& rot, float scale)
{
	auto ent = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Pos = pos;
	tf.Rot = rot;
	tf.Scale = { scale, scale, scale };

	auto& render = _ecs->emplace<RenderComp>(ent);
	render.ShaderId = RenderModule::EShaderId::Lit;
	render.Mat = { 0.5f, 0.1f, 0.5f, 30 };

	_engine->Render().ImportModel("./Models/duck.obj", 1.0f, render);
	_engine->Render().ImportTexture(L"./Models/duck.dds", render);
}
