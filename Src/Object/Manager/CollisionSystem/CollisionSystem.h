#pragma once
#include<vector>
#include<cstdint>
#include<algorithm>
#include<DxLib.h>
#include"../../Actor/Collider/ColliderInfo.h"
#include"../MoveMentSystem/MovementSystem.h"

using ColliderID =uint32_t;

struct AABB
{
	VECTOR min;
	VECTOR max;
};

struct HitInfo
{
	bool hasHit_ = false;
	VECTOR normal = { 0.0f,0.0f,0.0f };
	float penetration_ = 0.0f;
};

struct CollisionEvent
{
	MovementSystem::EntityID entityA;
	MovementSystem::EntityID entityB;
	ColliderID colA;
	ColliderID colB;
	bool isTrigger;
};

class CollisionSystem
{
public:
	std::vector<ColliderShapeData>shapes_;
	std::vector<ColliderInfo>colInfos_;
	std::vector<MovementSystem::EntityID>ownerEntities_;
	std::vector<AABB>swepBounds_;

	std::vector<ColliderID>idxToColliderID_;
	std::vector<uint32_t>colliderIDToidx_;




};

