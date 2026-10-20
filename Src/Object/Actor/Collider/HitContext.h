#pragma once
#include<DxLib.h>
#include<cstdint>

class ActorBase;
struct Collider;


struct HitContext
{
	ActorBase* otherActor = nullptr; // 接触相手
	const Collider* myCollider = nullptr; // 自分のコライダー
	const Collider* otherCollider = nullptr; // 相手のコライダー
	uint32_t        mySubIndex = 0;       // 自分のコライダー部位番号
	uint32_t        otherSubIndex = 0;       // 相手のコライダー部位番号
	bool isTrigger = false;

	VECTOR          hitPoint = { 0.0f, 0.0f, 0.0f }; // エフェクト発生・UI用
};