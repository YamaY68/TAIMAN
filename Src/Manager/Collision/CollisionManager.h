#pragma once
#include<DxLib.h>
#include<vector>
#include"../../Object/Common/Transform.h"
#include"../../Object/Actor/Collider/Collider.h"
#include"../../Object/Actor/Collider/HitContext.h"

class ActorBase;

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


class CollisionManager
{
public:
	CollisionManager(void);
	~CollisionManager(void);
	void RegisterActor(ActorBase* actor, uint32_t ownerID);
	void UnregisterActor(uint32_t ownerID);
	void Clear(void);
	void Update(void);




private:
	std::vector<ColliderEntry> entries_;
};

