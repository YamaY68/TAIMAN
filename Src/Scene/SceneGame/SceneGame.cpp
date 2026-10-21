#include "SceneGame.h"
#include"../../Manager/Scene/SceneManager.h"
#include"../../Manager/Input/KeyManager.h"

#include"../../Manager/Collision/CollisionManager.h"
#include"../../Object/Factory/FactoryManager.h"
#include"../../Object/Manager/ActorManager/ActorManager.h"
#include"../../Object/Manager/TimeManager/TimeManager.h"

SceneGame::SceneGame(void)
{
}

SceneGame::~SceneGame(void)
{
}

void SceneGame::SubLoad(void)
{
	actorManager_ = std::make_unique<ActorManager>();
	factoryManager_ = std::make_unique<FactoryManager>();
	collisionManager_ = std::make_unique<CollisionManager>();
	timeManager_ = std::make_unique<TimeManager>();

	actorManager_->Load();
}


void SceneGame::SubInit(void)
{
	actorManager_->Init();
}

void SceneGame::SubUpdate(void)
{
	timeManager_->Update();
	actorManager_->Update();
	collisionManager_->Update();

	if (KEY::GetInstanec().GetInfo(KEY::KEY_TYPE::DEBUG_KEY_0).down)
	{
		SceneManager::GetInstance().ChangeScene(SCENE_ID::GAMEOVER);
	}
}

void SceneGame::SubDraw(void)
{
	actorManager_->Draw();
	DrawFormatString(0, 0, 0xffffff, "Game");
}

void SceneGame::SubRelease(void)
{
}
