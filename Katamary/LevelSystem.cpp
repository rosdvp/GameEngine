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

	//CreateObstacle({ 0, 1.0f, 10 }, 
	//	"./Models/duck.obj", L"./Models/duck.dds", 
	//	0.2f);
}

void LevelSystem::CreateLight()
{
	auto ent = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Rot.Set(0, 70, 0);

	auto& light = _ecs->emplace<RenderLightComp>(ent);
	light.Color = Color::White();
	light.AmbientIntensity = 1.0f;
}

void LevelSystem::CreateGround()
{
	auto ent = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Scale = { 100, 1, 100 };

	auto& render = _ecs->emplace<RenderComp>(ent);
	render.ShaderId = RenderModule::EShaderId::SimpleUnlit;
	GeometryRenderBuilder::BuildCube(render, Color::Grey());
}

void LevelSystem::CreateObstacle(Vector3 pos)
{
	auto ent = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Pos = pos;

	auto& render = _ecs->emplace<RenderComp>(ent);
	render.ShaderId = RenderModule::EShaderId::SimpleLit;
	render.Mat = { 0.5f, 0.1f, 0.5f, 30 };
	//render.Mat = { 0.2775f, 0.23125f, 0.773911f, 89.6 };

	GeometryRenderBuilder::BuildCube(render, Color::Green());
}


void LevelSystem::CreateObstacle(Vector3 pos, std::string pathModelFile, std::wstring pathTextureFile, float scaleFactor)
{
	auto ent = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Pos = pos;
	tf.Rot.AddLocal(0, 90, 0);
	//tf.Scale = { 0.3f, 0.3f, 0.3f };
	//tf.Scale = { 0.5f, 0.5f, 0.5f };

	auto& render = _ecs->emplace<RenderComp>(ent);
	render.ShaderId = RenderModule::EShaderId::Lit;

	_engine->Render().ImportModel(pathModelFile, scaleFactor, render);
	_engine->Render().ImportTexture(pathTextureFile, render);
}