#include "TestFactory.h"
#include"../../Actor/ActorBase.h"
#include"../../Actor/Charactor/Player/SowrdsMan/SwordsMan.h"
#include"../../Actor/Stage/Floor/Floor.h"
#include"../../Actor/Stage/SkyDome/SkyDome.h"

TestFactory::TestFactory(void)
{
}

TestFactory::~TestFactory(void)
{
}

std::vector<std::unique_ptr<ActorBase>> TestFactory::Create(void)
{
	std::vector<std::unique_ptr<ActorBase>>actors;
	actors.push_back(std::make_unique<SwordsMan>());
	actors.push_back(std::make_unique<Floor>());
	actors.push_back(std::make_unique<SkyDome>());
	return actors;
}
