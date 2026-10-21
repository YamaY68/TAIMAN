#include "TestFactory.h"
#include"../../Actor/ActorBase.h"

TestFactory::TestFactory(void)
{
}

TestFactory::~TestFactory(void)
{
}

std::vector<std::unique_ptr<ActorBase>> TestFactory::Create(void)
{
	return std::vector<std::unique_ptr<ActorBase>>();
}
