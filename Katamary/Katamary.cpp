#include "BlahEngine.h"
#include "GeometryRenderBuilder.h"
#include "LevelSystem.h"
#include "PlayerCameraSystem.h"
#include "PlayerSystem.h"

using namespace BlahEngine;

class TestSystem : public IInitSystem, IRunSystem
{
public:
	entt::entity _ent;
	Vector3 _dir;

	void Init() override
	{
		_ent = _ecs->create();

		auto& tf = _ecs->emplace<TransformComp>(_ent);
		tf.Pos = {0, 5, 10};

		auto& render = _ecs->emplace<RenderComp>(_ent);
		render.ShaderId = RenderModule::EShaderId::SimpleLit;
		GeometryRenderBuilder::BuildCube(render, Color::Green());

		_dir = { 1, 0, 0 };
	}

	void Run()
	{
		float deltaTime = _engine->Time().GetFrameDeltaSecs();

		auto& tf = _ecs->get<TransformComp>(_ent);

		if (tf.Pos.X < -6)
			_dir = { 1, 0, 0 };
		if (tf.Pos.X > 6)
			_dir = { -1, 0, 0 };

		tf.Pos += _dir * deltaTime * 5;
	}
};


int main()
{
	Engine engine;
	engine.Init();
	
	engine.Systems().AddSystem(new LevelSystem);
	engine.Systems().AddSystem(new PlayerSystem);
	engine.Systems().AddSystem(new PlayerCameraSystem);
	engine.Systems().AddSystem(new TestSystem);

	engine.Run();

	return 0;
}