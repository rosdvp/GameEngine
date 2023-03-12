#pragma once

namespace BlahEngine
{
class Engine;
class EcsCore;

class ISystem
{
public:
	virtual ~ISystem() = default;

	void AttachEngine(Engine* engine, EcsCore* ecs)
	{
		_engine = engine;
		_ecs = ecs;
	}

protected:
	Engine* _engine = nullptr;
	EcsCore* _ecs = nullptr;
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
