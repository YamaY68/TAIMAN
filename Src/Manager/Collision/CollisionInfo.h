#pragma once
#include<DxLib.h>
#include<cstdint>

class ActorBase;
struct Collider;

struct CollisionResult
{

	bool   isHit = false;
	VECTOR normal = { 0.0f, 0.0f, 0.0f }; // B から A へ向かう押し出し法線
	float  penetration = 0.0f;                 // めり込み深さ
	VECTOR hitPoint = { 0.0f, 0.0f, 0.0f }; // 接触した点
};

struct ColliderEntry
{
	uint32_t   ownerID;    // アクターの一意なID
	ActorBase* ownerActor; // アクター本体
	Collider* collider;   // コライダー本体への参照
	uint32_t   subIndex;   // 部位インデックス
};

enum class CollisionPairType :uint8_t
{
	NONE = 0,
	SPHERE_SPHERE,
	SPHERE_CUBE,
	SPHERE_CAPSULE,
	CAPSULE_CAPSULE,
	CAPSULE_CUBE,
	CUBE_CUBE,
	MAX,
};