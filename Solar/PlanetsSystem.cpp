#include "PlanetsSystem.h"

#include "GeometryRenderBuilder.h"
#include "PlanetComp.h"

using namespace BlahEngine;


void PlanetsSystem::Init()
{
	auto sun = CreatePlanet(false, {}, 
		{ 1, 0, 0 }, 0, 0, 
		1, {3, 3, 3}, Color::Yellow()
	);
	auto mercury = CreatePlanet(true, sun,
		{ 1, 0, 0 }, 5, 20, 
		1.2f, {0.5f,0.5f, 0.5f}, Color::Red()
	);
	auto venus = CreatePlanet(true, sun,
		{ 1, 0, 0 }, 10, 17,
		1.3f, { 1 ,1, 1 }, Color::Purple()
	);
	auto earth = CreatePlanet(true, sun,
		{ 1, 0, 0 }, 15, 15,
		1.4f, { 1 ,1, 1 }, Color::Blue()
	);
	auto moon = CreatePlanet(true, earth,
		{ 1, 0, 0 }, 2.0f, 100,
		1.5f, { 0.3f ,0.3f, 0.3f }, Color::Grey()
	);
	auto mars = CreatePlanet(true, sun,
		{ 1, 0, 0 }, 20, 10,
		1.6f, { 1.1f,1.1f, 1.1f }, Color::Orange()
	);
	auto phobos = CreatePlanet(true, mars,
		{ 1, 0, 0 }, 2.0f, 100,
		1.7f, { 0.3f ,0.3f, 0.3f }, Color::Grey()
	);
	auto deimos = CreatePlanet(true, mars,
		{ 1, 0, 0 }, 3.0f, 50,
		1.8f, { 0.3f ,0.3f, 0.3f }, Color::Grey()
	);
	auto jupiter = CreatePlanet(true, sun,
		{ 1, 0, 0 }, 25, 18,
		1.9f, { 2.0f,2.0f, 2.0f }, Color(0.7f, 0.5f, 0.5f, 1.0f)
	);
	auto saturn = CreatePlanet(true, sun,
		{ 1, 0, 0 }, 30, 30,
		2.0f, { 2.0f,2.0f, 2.0f }, Color(0.5f, 0.5f, 0.7f, 1.0f)
	);
	auto uranus = CreatePlanet(true, sun,
		{ 1, 0, 0 }, 37, 15,
		2.1f, { 1.5f,1.5f, 1.5f }, Color(0.5f, 0.7f, 0.5f, 1.0f)
	);
	auto neptune = CreatePlanet(true, sun,
		{ 1, 0, 0 }, 45, 25,
		2.2f, { 1.3f,1.3f, 1.3f }, Color(0.3f, 0.3f, 0.3f, 1.0f)
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
	GeometryRenderBuilder::BuildSphere(render, 20, color);
	render.DrawerId = 0;
	render.ShaderId = 0;
	for (int i = 0; i < render.VerticesCount; i += 10)
		render.Vertices[i].Color = Color::White();

	return entity;
}
