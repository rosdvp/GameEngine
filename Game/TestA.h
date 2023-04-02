#pragma once

#include "entt.hpp"
#include "TestB.h"

class TestA
{
	TestA()
	{
		TestB b();
		entt::registry reg;
	}
};
