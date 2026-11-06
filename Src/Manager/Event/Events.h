#pragma once
#include<DxLib.h>
#include<cstdint>
using EntityID = uint32_t;

struct LockonToggleEvent
{
	VECTOR playerPos_;
};

struct LockonChangeTargetEvent
{
	VECTOR playerPos_;
	EntityID currentTargetId_ = 0;
};

struct BulletTimeEvent
{
	float timeScale_;
	float duration_;
};