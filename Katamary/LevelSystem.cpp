#include "LevelSystem.h"

#include "GeometryRenderBuilder.h"
#include "RenderComp.h"

using namespace BlahEngine;

void LevelSystem::Init()
{
	CreateGround();
	CreateObstacle({ 5, 1.0f, 5 });
	CreateObstacle({ 5, 1.0f, -5 });
	CreateObstacle({ -5, 1.0f,  5 });
	CreateObstacle({ -5, 1.0f, -5 });
}

void LevelSystem::CreateGround()
{
	auto ent = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Scale = { 100, 1, 100 };

	auto& render = _ecs->emplace<RenderComp>(ent);
	render.DrawerId = 0;
	render.ShaderId = 0;
	GeometryRenderBuilder::BuildCube(render, Color::Grey());
}

void LevelSystem::CreateObstacle(Vector3 pos)
{
	auto ent = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Pos = pos;

	auto& render = _ecs->emplace<RenderComp>(ent);
	render.DrawerId = 0;
	render.ShaderId = 0;
	GeometryRenderBuilder::BuildCube(render, Color::Red());
}
