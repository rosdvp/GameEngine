#pragma once

#include "entt.hpp"

namespace BlahEngine
{
class Engine;

class ISystem
{
public:
	virtual ~ISystem() = default;

	void AttachEngine(Engine* engine, entt::registry* ecs)
	{
		_engine = engine;
		_ecs = ecs;
	}

protected:
	Engine* _engine = nullptr;
	entt::registry* _ecs = nullptr;
};

class IInitSystem : virtual public ISystem
{
public:
	virtual void Init() = 0;
};

class IRunSystem : virtual public ISystem
{
public:
	virtual void Run() = 0;
};
}
