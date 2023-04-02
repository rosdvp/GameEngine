#include "PlanetsSystem.h"

#include "GeometryRenderBuilder.h"
#include "PlanetComp.h"

using namespace BlahEngine;


void PlanetsSystem::Init()
{
	auto sun = CreatePlanet(entt::null,
		0, 0, 1,
		{ 3, 3, 3 }, Color::Yellow()
	);
	auto mercury = CreatePlanet(sun,
		5, 20, 1.2f,
		{ 0.5f, 0.5f, 0.5f }, Color::Red()
	);
	
	auto venus = CreatePlanet(sun,
		10, 17, 1.3f,
		{ 1, 1, 1 }, Color::Purple()
	);
	auto earth = CreatePlanet(sun,
		15, 15, 1.4f,
		{ 1, 1, 1 }, Color::Blue()
	);
	auto moon = CreatePlanet(earth,
		2, 100, 1.5f,
		{ 0.3f, 0.3f, 0.3f }, Color::Grey());
	
	auto mars = CreatePlanet(sun,
		20, 10,1.6f,
		{ 1.1f,1.1f, 1.1f }, Color::Orange()
	);
	auto phobos = CreatePlanet(mars,
		2.0f, 100,1.7f, 
		{ 0.3f ,0.3f, 0.3f }, Color::Grey()
	);
	auto deimos = CreatePlanet(mars,
		3.0f, 50, 1.8f, 
		{ 0.3f ,0.3f, 0.3f }, Color::Grey()
	);
	auto jupiter = CreatePlanet(sun,
		25, 18,1.9f, 
		{ 2.0f,2.0f, 2.0f }, Color(0.7f, 0.5f, 0.5f, 1.0f)
	);
	auto saturn = CreatePlanet(sun,
	30, 30,2.0f, 
		{ 2.0f,2.0f, 2.0f }, Color(0.5f, 0.5f, 0.7f, 1.0f)
	);
	auto uranus = CreatePlanet(sun,
		37, 15, 2.1f, 
		{ 1.5f,1.5f, 1.5f }, Color(0.5f, 0.7f, 0.5f, 1.0f)
	);
	auto neptune = CreatePlanet(sun,
		45, 25,2.2f, 
		{ 1.3f,1.3f, 1.3f }, Color(0.3f, 0.3f, 0.3f, 1.0f)
	);
}

void PlanetsSystem::Run()
{
	float deltaTime = _engine->Time().GetFrameDeltaSecs();

	auto view = _ecs->view<TransformComp, const PlanetComp>();

	for (auto ent : view)
	{
		auto [tf, planet] = view.get<TransformComp, const PlanetComp>(ent);

		tf.Pos = tf.Pos.GetRotY(planet.OrbitSpeed * deltaTime);
		tf.Rot.Y += planet.SelfRotateSpeed * deltaTime;
	}
}

entt::entity PlanetsSystem::CreatePlanet(
	entt::entity entParent,
	float orbitRadius, float orbitSpeed, 
	float selfRotateSpeed, const Vector3& scale, const Color& color)
{
	auto ent = _ecs->create();
	auto& tf = _ecs->emplace<TransformComp>(ent);
	tf.Pos = { orbitRadius, 0, 0 };
	tf.Scale = scale;
	tf.Parent = entParent;

	auto& planet = _ecs->emplace<PlanetComp>(ent);
	planet.OrbitSpeed = orbitSpeed;
	planet.SelfRotateSpeed = selfRotateSpeed;

	auto& render = _ecs->emplace<RenderComp>(ent);
	GeometryRenderBuilder::BuildSphere(render, 20, color);
	render.DrawerId = 0;
	render.ShaderId = 0;
	for (int i = 0; i < render.VerticesCount; i += 10)
		render.Vertices[i].Color = Color::White();

	return ent;
}
