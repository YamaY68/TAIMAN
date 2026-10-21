#include "FactoryManager.h"

FactoryManager::FactoryManager(void)
{
}

FactoryManager::~FactoryManager(void)
{
}

std::vector<std::unique_ptr<ActorBase>> FactoryManager::Create(FACTORY_TYPE type)
{
	auto it = factories_.find(type);
	if (it != factories_.end() && it->second)
	{
		return it->second->Create();
	}
	return {};
}
