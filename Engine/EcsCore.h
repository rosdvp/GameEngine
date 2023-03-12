#pragma once
#include <unordered_map>
#include "CompPool.h"
#include "EntitiesPool.h"
#include "Filter.h"
#include "FilterMask.h"

namespace BlahEngine
{
class EcsCore
{
public:
	EcsCore();

	~EcsCore();


	Entity CreateEntity();
	void DestroyEntity(Entity entity);
	bool IsEntityExist(Entity entity) const;

	template<typename T>
	T& AddComp(Entity entity)
	{
		T& comp = GetCompPool<T>()->Add(entity);

		_entitiesCompsMap[entity] |= CompPool<T>::GetId();

		auto filters = _filtersByComps[CompPool<T>::GetId()];
		for (auto filter : filters)
			filter->ValidateEntity(entity);

		return comp;
	}

	template<typename T>
	T& GetComp(Entity entity)
	{
		return GetCompPool<T>()->Get(entity);
	}

	template<typename T>
	void RemoveComp(Entity entity)
	{
		GetCompPool<T>()->Remove(entity);

		_entitiesCompsMap[entity] &= ~CompPool<T>::GetId();
		if (_entitiesCompsMap[entity] == 0)
			_entitiesPool.Destroy(entity);

		CompId compId = CompPool<T>::GetId();
		auto filters = _filtersByComps[compId];
		for (auto filter : filters)
			filter->ValidateEntity(entity);
	}

	template<typename T>
	bool HasComp(Entity entity)
	{
		return GetCompPool<T>()->Has(entity);
	}

	bool HasComp(Entity entity, CompId compId)
	{
		auto pool = _comps[compId];
		return pool != nullptr && pool->Has(entity);
	}


	Filter* GetFilter(FilterMask mask);

private:

	EntitiesPool _entitiesPool;

	std::unordered_map<Entity, CompId> _entitiesCompsMap;

	std::unordered_map<CompId, ICompPool*> _comps;

	std::unordered_map<FilterMask, Filter*> _filters;
	std::unordered_map<CompId, std::vector<Filter*>> _filtersByComps;


	template<typename T>
	CompPool<T>* GetCompPool()
	{
		CompId id = CompPool<T>::GetId();
		if (_comps[id] == nullptr)
		{
			auto pool = new CompPool<T>(8, 8);
			_comps[id] = pool;
		}
		return reinterpret_cast<CompPool<T>*>(_comps[id]);
	}

public:
#ifdef _DEBUG
	ICompPool* GetCompPoolById(CompId id);
	void DebugPrint();
#endif
};
}
