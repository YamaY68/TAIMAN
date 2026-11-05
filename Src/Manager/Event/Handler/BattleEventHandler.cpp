#include "BattleEventHandler.h"
#include"../EventManager.h"
#include"../Events.h"
#include"../../../Object/Manager/TimeManager/TimeManager.h"
#include"../../../Object/Actor/Camera/CameraManager.h"

void BattleEventHandler::Init(GameContext* context)
{
	auto& ev = EventManager::GetInstance();
	ev.Subscribe<BulletTimeEvent>(this, [this](const BulletTimeEvent& e) {
		if (timeManager_)timeManager_->StartBulletTime(e.duration_, e.timeScale_);
		});
}

void BattleEventHandler::Release(void)
{
	EventManager::GetInstance().Unsubscribe(this);
}
