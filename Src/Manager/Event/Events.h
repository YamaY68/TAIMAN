#pragma once
#include<DxLib.h>
#include<cstdint>

struct LockonEvent
{
	VECTOR playerPos_;
};

struct BulletTimeEvent
{
	float timeScale_;
	float duration_;
};