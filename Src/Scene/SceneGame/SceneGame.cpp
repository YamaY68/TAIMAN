#include "SceneGame.h"
#include"../../Manager/Scene/SceneManager.h"
#include"../../Manager/Input/KeyManager.h"

#include"../../Manager/Collision/CollisionManager.h"
#include"../../Object/Factory/FactoryManager.h"
#include"../../Object/Manager/ActorManager/ActorManager.h"
#include"../../Object/Manager/TimeManager/TimeManager.h"
#include"../../Object/Actor/Camera/CameraManager.h"

#include"../../Object/Actor/Camera/PlayerFollowVirtualCamera.h"
#include"../../Object/Actor/Camera/DebugFreeVirtualCamera.h"


#include"Game/GameModeBase.h"
#include"Game/GameModeBoss.h"

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
	cameraManager_ = std::make_unique<CameraManager>();


	actorManager_->SetCollisionManager(collisionManager_.get());


	//actorManager_->Load();

}


void SceneGame::SubInit(void)
{
	//actorManager_->Init();

	auto debugCam = std::make_unique<DebugFreeVirtualCamera>(999);
	cameraManager_->AddCamera(debugCam.get());

	ChangeMode(std::make_unique<GameModeBoss>(this));
}

void SceneGame::SubUpdate(void)
{
	timeManager_->Update();
	actorManager_->Update();
	collisionManager_->Update();
	actorManager_->PostUpdate();
	cameraManager_->Update();

	if (currentMode_)
	{
		currentMode_->Update();

		if (currentMode_->IsCleared())
		{
			//ChangeMode(std::make_unique<GameModeBoss>(this));
		}
	}


	if (KEY::GetInstance().GetInfo(KEY::KEY_TYPE::DEBUG_KEY_0).down)
	{
		SceneManager::GetInstance().ChangeScene(SCENE_ID::GAMEOVER);
	}
}

void SceneGame::SubDraw(void)
{
	cameraManager_->ApplyDxLib();

	actorManager_->Draw();

	UpdateEffekseer3D();
	DrawEffekseer3D();

	DrawFormatString(0, 0, 0xffffff, "Game");
}

void SceneGame::SubRelease(void)
{
	if (currentMode_)
	{
		currentMode_->Exit();
		currentMode_.reset();
	}

	if (actorManager_)     actorManager_->Release();
	if (collisionManager_) collisionManager_->Clear();
	if (factoryManager_)   factoryManager_->Clear();
}

void SceneGame::ChangeMode(std::unique_ptr<GameModeBase> nextMode)
{
	if (currentMode_)
	{
		currentMode_->Exit();
	}
	currentMode_ = std::move(nextMode);
	if (currentMode_)
	{
		currentMode_->Enter();
	}
}
