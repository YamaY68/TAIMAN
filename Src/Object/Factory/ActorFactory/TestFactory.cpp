#include "TestFactory.h"
#include"../../Actor/ActorBase.h"
#include"../../Actor/Charactor/Player/SowrdsMan/SwordsMan.h"
#include"../../Actor/Stage/Floor/Floor.h"
#include"../../Actor/Stage/SkyDome/SkyDome.h"
#include"../../Actor/Camera/CameraManager.h"

TestFactory::TestFactory(CameraManager* cameraManager)
{
	cameraManager_ = cameraManager;
}

TestFactory::~TestFactory(void)
{
}

std::vector<std::unique_ptr<ActorBase>> TestFactory::Create(void)
{
	std::vector<std::unique_ptr<ActorBase>>actors;
	actors.push_back(std::make_unique<Floor>());
	actors.push_back(std::make_unique<SkyDome>());
	actors.push_back(std::make_unique<SwordsMan>(cameraManager_));
	return actors;
}
