#include "pch.h"
#include "EcsCore.h"

#include "FilterMask.h"

using namespace BlahEngine;

EcsCore::EcsCore() :
	_entitiesPool(16)
{}

EcsCore::~EcsCore()
{
	for (auto pair : _filters)
		delete pair.second;
	for (auto pair : _comps)
		delete pair.second;
}

Entity EcsCore::CreateEntity()
{
	Entity entity = _entitiesPool.Create();
	_entitiesCompsMap[entity] = 0;
	return entity;
}

void EcsCore::DestroyEntity(Entity entity)
{
	_entitiesPool.Destroy(entity);

	CompId entityComps = _entitiesCompsMap[entity];
	CompId compId = 1;
	for (int i = 0; i < 32; i++)
	{
		if ((entityComps & compId) == compId)
		{
			_comps[compId]->Remove(entity);
			for (auto filter : _filtersByComps[compId])
				filter->ValidateEntity(entity);
		}
		compId *= 2;
	}
	_entitiesCompsMap[entity] = 0;
}

bool EcsCore::IsEntityExist(Entity entity) const
{
	return _entitiesPool.IsExist(entity);
}

Filter* EcsCore::GetFilter(FilterMask mask)
{
	if (_filters[mask] == nullptr)
	{
		std::vector<CompId> incCompsIds;
		std::vector<CompId> excCompsIds;

		CompId compId = 1;
		for (int i = 0; i < 32; i++)
		{
			if ((mask.IncIds & compId) == compId)
				incCompsIds.push_back(compId);
			if ((mask.ExcIds & compId) == compId)
				excCompsIds.push_back(compId);
			compId *= 2;
		}

		auto filter = new Filter(this, incCompsIds, excCompsIds, 8, 8);
		_filters[mask] = filter;

		for (auto id : incCompsIds)
			_filtersByComps[id].push_back(filter);
		for (auto id : excCompsIds)
			_filtersByComps[id].push_back(filter);

		for (auto& pair : _entitiesCompsMap)
		{
			if ((pair.second & mask.IncIds) != 0)
				filter->ValidateEntity(pair.first);
		}
	}
	return _filters[mask];
}

#ifdef _DEBUG
ICompPool* EcsCore::GetCompPoolById(CompId id)
{
	return _comps[id];
}

void EcsCore::DebugPrint()
{
	_entitiesPool.DebugPrint();
	for (auto pair : _comps)
		if (pair.second != nullptr)
			pair.second->DebugPrint();
	for (auto pair : _filters)
		pair.second->DebugPrint();
}
#endif
