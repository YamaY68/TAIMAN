#include "GameModeBoss.h"
#include "../SceneGame.h"
#include"../../../Object/Actor/Camera/CameraManager.h"
#include"../../../Object/Actor/Camera/DebugFreeVirtualCamera.h"
#include"../../../Object/Manager/ActorManager/ActorManager.h"
#include"../../../Object/Factory/ActorFactory/TestFactory.h"
#include"../../../Manager/Collision/CollisionManager.h"
#include"../../../Object/Factory/FactoryManager.h"

GameModeBoss::GameModeBoss(SceneGame* game)
	: GameModeBase(game)
{
}

void GameModeBoss::Enter(void)
{
	auto factory = std::make_unique<TestFactory>();
	game_->GetFactoryManager()->RegisterFactory(FACTORY_TYPE::PLAYER, std::move(factory));
	auto actors = game_->GetFactoryManager()->Create(FACTORY_TYPE::PLAYER);
	auto entityIDs = game_->GetActorManager()->RegisterActors(std::move(actors));
}

void GameModeBoss::Update(void)
{
}

void GameModeBoss::Exit(void)
{
}

bool GameModeBoss::IsCleared(void) const
{
	return false;
}

void GameModeBoss::CreateActor(void)
{
}