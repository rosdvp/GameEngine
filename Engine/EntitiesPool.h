#pragma once
#include "Entity.h"

namespace BlahEngine
{
class EntitiesPool
{
public:
	EntitiesPool(int baseSize);
	~EntitiesPool();

	Entity Create();
	void Destroy(Entity entity);
	bool IsExist(Entity entity) const;

private:
	int _size;
	int _count;

	bool* _existMap;

	int* _freeIndices;
	int _freeIndicesCount;

	void ValidateSize();

public:
#ifdef _DEBUG
	void DebugPrint() const;
#endif
};
}