#pragma once

#include "entt.hpp"

#include "TestA.h"

class TestB
{
	TestB()
	{
		TestA a();
		entt::registry reg;
	}
};