#include "LevelSystem.h"

#include "GeometryRenderBuilder.h"
#include "RenderComp.h"
#include "RenderLightComp.h"
#include "RenderMaterialsPresets.h"

using namespace BlahEngine;

void LevelSystem::Init()
{
	CreateGround();
	//CreateObstacle({ 5, 1.0f, 5 }, {1, 1, 1});
	//CreateObstacle({ 5, 1.5f, -5 }, {1, 2, 1});
	//CreateObstacle({ -5, 2.0f, -5 }, {1, 3, 1});
	//CreateObstacle({ -5, 2.5f, 5 }, {1, 4, 1});

	//_entDebugBox = CreateObstacle({ 0, 1.0f, 8.0f }, { 1, 1, 1 });
	//_ecs->get<CollisionComp>(_entDebugBox).IsStatic = false;

	//CreateDuck({ 0, 0.5f, 5 }, { 0, 0, 0 }, 0.3f);

	int rowsCount = 10;
	int columnsCount = 5;
	int spaceX = 5;
	int spaceZ = 5;
	float scaleBase = 0.25f;
	float scaleStep = 0.25f;

	float posZ = 3;

	for (int row = 0; row < rowsCount; row++)
	{
		float scale = scaleBase + scaleStep * row;

		float posY = 0.5f + scale / 2.0f;

		for (int column = 0; column < columnsCount; column++)
		{
			float posX = (column - columnsCount / 2.0f) * (scale + spaceX);

			CreateObstacle({ posX, posY, posZ }, { scale, scale, scale });
		}

		posZ += scale + spaceZ;
	}


	CreateDuck({ -5, 0.5f, -5 }, { 0, 0, 0 }, 1.0f);
	CreateObstacle({ 5, 1, -5 }, { 1, 1, 1 });
}

void LevelSystem::Run()
{
	//auto& tf = _ecs->get<TransformComp>(_entDebugBox);
	//
	//tf.Pos.Z -= 1 * _engine->Time().GetFrameDeltaSecs();
}

void LevelSystem::CreateGround()
{
	auto ent = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Scale = { 100, 1, 100 };

	auto& render = _ecs->emplace<RenderComp>(ent);
	render.ShaderId = RenderModule::EShaderId::SimpleLit;
	render.Mat = MatPresetChrome;
	GeometryRenderBuilder::BuildCube(render, Color::Grey());
}

entt::entity LevelSystem::CreateObstacle(Vector3 pos, Vector3 scale)
{
	auto ent = _ecs->create();

	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Pos = pos;
	tf.Scale = scale;

	auto& render = _ecs->emplace<RenderComp>(ent);
	render.ShaderId = RenderModule::EShaderId::SimpleLit;
	render.Mat = MatPresetChrome;
	GeometryRenderBuilder::BuildCube(render, Color::Green());

	auto& col = _ecs->emplace<CollisionComp>(ent);
	col.Type = CollisionComp::BoxCollision;
	col.Size = { 1, 1, 1 };
	col.IsStatic = true;

	return ent;
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
	render.Mat = MatPresetChrome;
	_engine->Render().ImportModel("./Models/duck2.obj", 1.0f, render);
	_engine->Render().ImportTexture(L"./Models/duck.dds", render);

	auto& col = _ecs->emplace<CollisionComp>(ent);
	col.Type = CollisionComp::BoxCollision;
	col.IsStatic = true;
	col.Size = { 0.4f, 0.5f, 0.7f };
}
