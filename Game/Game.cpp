#include <iostream>

#include "entt.hpp"
#include "TestA.h"
#include "TestB.h"

struct Comp
{
	std::unique_ptr<int[]> Arr;

	Comp(Comp&& other) = default;
	Comp& operator=(Comp&& other) = default;

	~Comp()
	{
		std::cout << "destructed " << Arr.get() << std::endl;
	}
};


int main()
{
	TestA testA();
	TestB testB();

	entt::registry reg;

	auto entA = reg.create();
	auto& compA = reg.emplace<Comp>(entA, std::make_unique<int[]>(1));
	compA.Arr[0] = 1;

	auto entB = reg.create();
	auto& compB = reg.emplace<Comp>(entB, std::make_unique<int[]>(2));
	compB.Arr[0] = 1;

	auto entC = reg.create();
	auto& compC = reg.emplace<Comp>(entC, std::make_unique<int[]>(3));
	compC.Arr[0] = 1;


	
	reg.view<const Comp>().each([](entt::entity ent, const Comp& comp)
	{
		std::cout << "ent: " << (int)ent << std::endl;
		std::cout << "val: " << comp.Arr[0] << std::endl;
		std::cout << "addr: " << comp.Arr << std::endl;
		std::cout << "---------" << std::endl;
	});

	std::cout << "########################" << std::endl;

	reg.destroy(entB);
	reg.view<const Comp>().each([](entt::entity ent, const Comp& comp)
		{
			std::cout << "ent: " << (int)ent << std::endl;
			std::cout << "val: " << comp.Arr[0] << std::endl;
			std::cout << "addr: " << comp.Arr << std::endl;
			std::cout << "---------" << std::endl;
		});

	std::cout << "########################" << std::endl;

	reg.destroy(entC);
	reg.view<const Comp>().each([](entt::entity ent, const Comp& comp)
		{
			std::cout << "ent: " << (int)ent << std::endl;
			std::cout << "val: " << comp.Arr[0] << std::endl;
			std::cout << "addr: " << comp.Arr << std::endl;
			std::cout << "---------" << std::endl;
		});
	
	return 0;
}