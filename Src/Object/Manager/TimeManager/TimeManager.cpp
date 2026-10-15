#include "TimeManager.h"
#include <DxLib.h>
#include"../../../Manager/Scene/SceneManager.h"

TimeManager* TimeManager::instance_ = nullptr;

TimeManager::TimeManager(void)
	:
	globalTimeScale_(1.0f),
	bulletTimeScale_(1.0f),
	bulletTimer_(0.0f),
	isBulletTime_(false)
{

}

void TimeManager::CreateInstance(void)
{
	if (!instance_) instance_ = new TimeManager();
}

TimeManager& TimeManager::GetInstance(void)
{
	return *instance_;
}

void TimeManager::DeleteInstance(void)
{
	if (instance_)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

float TimeManager::GetTimeScale(void)
{
	return bulletTimeScale_;
}

void TimeManager::Update(void)
{
	if (bulletTimer_ > 0)
	{
		bulletTimer_--;
		if (bulletTimer_ <= 0)
		{
			bulletTimer_ = 0;
			bulletTimeScale_ = 1.0f;
			isBulletTime_ = false;
		}
	}
}
