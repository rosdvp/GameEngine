#include "LevelSystem.h"

#include "GeometryRenderBuilder.h"
#include "RenderComp.h"
#include "RenderLightComp.h"

using namespace BlahEngine;

void LevelSystem::Init()
{
	CreateLight();
	CreateGround();
	CreateObstacle({ 5, 1.0f, 5 });
	CreateObstacle({ 5, 1.0f, -5 });
	CreateObstacle({ -5, 1.0f,  5 });
	CreateObstacle({ -5, 1.0f, -5 });

	CreateObstacle({ 0, 2, 10 }, "./Models/duck.obj", L"./Models/duck.dds");
}

void LevelSystem::CreateLight()
{
	auto ent = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Rot.Set(0, 70, 0);

	auto& light = _ecs->emplace<RenderLightComp>(ent);
	light.Color = Color::White();
}

void LevelSystem::CreateGround()
{
	auto ent = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Scale = { 100, 1, 100 };

	auto& render = _ecs->emplace<RenderComp>(ent);
	render.DrawerId = 0;
	render.ShaderId = RenderModule::EShaderId::SimpleUnlit;
	GeometryRenderBuilder::BuildCube(render, Color::Grey());
}

void LevelSystem::CreateObstacle(Vector3 pos)
{
	auto ent = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Pos = pos;

	auto& render = _ecs->emplace<RenderComp>(ent);
	render.DrawerId = 0;
	render.ShaderId = RenderModule::EShaderId::SimpleLit;
	GeometryRenderBuilder::BuildCube(render, Color::Green());
}


void LevelSystem::CreateObstacle(Vector3 pos, std::string pathModelFile, std::wstring pathTextureFile)
{
	auto ent = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Pos = pos;
	tf.Rot.AddLocal(0, 90, 0);
	tf.Scale = { 0.3f, 0.3f, 0.3f };

	auto& render = _ecs->emplace<RenderComp>(ent);
	render.DrawerId = 0;
	render.ShaderId = RenderModule::EShaderId::Lit;

	_engine->Render().ImportModel(pathModelFile, render);
	_engine->Render().ImportTexture(pathTextureFile, render);
}