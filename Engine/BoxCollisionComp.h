#pragma once
#include "Vector3.h"

namespace BlahEngine
{
struct BoxCollisionComp
{
	Vector3 Size = { 1, 1, 1 };

	void Release()
	{
		Size = { 1, 1, 1 };
	}
};
}
