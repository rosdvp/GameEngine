#include "PlanetsSystem.h"

#include "GeometryRenderBuilder.h"
#include "PlanetComp.h"

using namespace BlahEngine;


void PlanetsSystem::Init()
{
	auto sun = CreatePlanet(false, {}, 
		{ 1, 0, 0 }, 0, 0, 
		1, {1, 1, 1}, Color::Yellow()
	);
	auto mercury = CreatePlanet(true, sun,
		{ 1, 0, 0 }, 5, 20, 
		0.9f, {0.5f,0.5f, 0.5f}, Color::Red()
	);
	auto venus = CreatePlanet(true, sun,
		{ 1, 0, 0 }, 10, 17,
		0.8f, { 1 ,1, 1 }, Color::Purple()
	);
	auto earth = CreatePlanet(true, sun,
		{ 1, 0, 0 }, 15, 15,
		0.7f, { 1 ,1, 1 }, Color::Blue()
	);
	auto moon = CreatePlanet(true, earth,
		{ 1, 0, 0 }, 2.0f, 100,
		0.6f, { 0.3f ,0.3f, 0.3f }, Color::Grey()
	);
	auto mars = CreatePlanet(true, sun,
		{ 1, 0, 0 }, 20, 10,
		0.5f, { 1.1f,1.1f, 1.1f }, Color::Orange()
	);
	auto phobos = CreatePlanet(true, mars,
		{ 1, 0, 0 }, 2.0f, 100,
		0.4f, { 0.3f ,0.3f, 0.3f }, Color::Grey()
	);
	auto deimos = CreatePlanet(true, mars,
		{ 1, 0, 0 }, 3.0f, 50,
		0.3f, { 0.3f ,0.3f, 0.3f }, Color::Grey()
	);

	_planetsFilter = _ecs->GetFilter(FilterMask().Inc<PlanetComp>().Inc<TransformComp>());
}

void PlanetsSystem::Run()
{
	float deltaTime = _engine->Time()->GetFrameDeltaSecs();

	for (auto entity : *_planetsFilter)
	{
		auto& tf = _ecs->GetComp<TransformComp>(entity);
		auto& planet = _ecs->GetComp<PlanetComp>(entity);

		if (planet.IsRootBounded)
		{
			auto rootPos = _ecs->GetComp<TransformComp>(planet.Root).Pos;
			planet.Dir = planet.Dir.GetRotY(planet.OrbitSpeed * deltaTime);
			tf.Pos = rootPos + planet.Dir * planet.OrbitRadius;
		}

		tf.Rot.Y += planet.SelfRotateSpeed * deltaTime;
	}
}

Entity PlanetsSystem::CreatePlanet(
	bool isRootBounded, Entity root,
	const Vector3& initDir,float orbitRadius, float orbitSpeed, 
	float selfRotateSpeed, const Vector3& scale, const Color& color)
{
	auto entity = _ecs->CreateEntity();
	auto& tf = _ecs->AddComp<TransformComp>(entity);
	tf.Scale = scale;

	auto& planet = _ecs->AddComp<PlanetComp>(entity);
	planet.IsRootBounded = isRootBounded;
	planet.Root = root;
	planet.Dir = initDir;
	planet.OrbitRadius = orbitRadius;
	planet.OrbitSpeed = orbitSpeed;
	planet.SelfRotateSpeed = selfRotateSpeed;

	auto& render = _ecs->AddComp<RenderComp>(entity);
	GeometryRenderBuilder::BuildSphere(render, 10, color);
	render.DrawerId = 0;
	render.ShaderId = 0;
	for (int i = 0; i < render.VerticesCount; i += 10)
		render.Vertices[i].Color = Color::White();

	return entity;
}
