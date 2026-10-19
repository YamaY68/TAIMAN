#pragma once
#include<DxLib.h>
#include<cstdint>
#include"../../Object/Actor/Collider/Collider.h"

enum class CollisionPairType :uint8_t
{
	NONE = 0,
	SPHERE_SPHERE,
	SPHERE_BOX,
	SPHERE_CAPSULE,
	BOX_BOX,
	BOX_CAPSULE,
	CAPSULE_CAPSULE,
	MAX,
};

class CollisionLogic
{
public:
	CollisionLogic(void);
	~CollisionLogic(void);

	static CollisionPairType GetCollisionPairType(ColliderShape shapeA, ColliderShape shapeB);

private:
};

