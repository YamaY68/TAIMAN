#include "TestFactory.h"

TestFactory::~TestFactory(void)
{
}

std::vector<std::unique_ptr<ActorBase>> TestFactory::Create(void)
{
	return std::vector<std::unique_ptr<ActorBase>>();
}
