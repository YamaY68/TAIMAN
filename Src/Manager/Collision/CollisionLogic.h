#pragma once
#include<DxLib.h>
#include<cstdint>
#include"../../Object/Actor/Collider/Collider.h"
#include"CollisionInfo.h"


class CollisionLogic
{
public:
	CollisionLogic(void);
	~CollisionLogic(void);

	static CollisionPairType GetCollisionPairType(CollisionShape shapeA, CollisionShape shapeB);
	static CollisionResult DispatchCollision(const CollisionPairType pairType, const Collider* colliderA, const Collider* colliderB);

private:
	static CollisionResult SphereToSphere(Collider* a, Collider* b);
	static CollisionResult SphereToCapsule(Collider* a, Collider* b);
	static CollisionResult SphereToCube(Collider* a, Collider* b);
	static CollisionResult CapsuleToCapsule(Collider* a, Collider* b);
	static CollisionResult CapsuleToCube(Collider* a, Collider* b);
	static CollisionResult CubeToCube(Collider* a, Collider* b);
};

