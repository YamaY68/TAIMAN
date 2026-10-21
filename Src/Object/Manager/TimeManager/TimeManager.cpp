#include "TimeManager.h"
#include <DxLib.h>
#include"../../../Manager/Scene/SceneManager.h"


TimeManager::TimeManager(void)
	:
	globalTimeScale_(1.0f),
	bulletTimeScale_(1.0f),
	bulletTimer_(0.0f),
	isBulletTime_(false)
{

}

TimeManager::~TimeManager(void)
{
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
