#pragma once
#include<DxLib.h>
#include<cstdint>
#include <initializer_list>

enum class ColliderLayer :uint8_t
{
	NONE,
	STAGE,
	ACTOR,
	PLAYER_ATTACK,
	ENEMY_ATTACK,

	MAX,
};

enum class ColliderShape :uint8_t
{
	NONE,
	BOX,
	SPHERE,
	CAPSULE,

	MAX,
};

struct ColliderShapeData
{
	ColliderShape shape = ColliderShape::NONE;
	union
	{
		struct { float radius; }sphere;
		struct { float radius; VECTOR localTop; VECTOR localDown; }capsule;
		struct { VECTOR halfSize; }box;
	};
	static ColliderShapeData Sphere(float radius)
	{
		ColliderShapeData data;
		data.shape = ColliderShape::SPHERE; data.sphere.radius = radius; return data;
	}
	static ColliderShapeData Capsule(float radiuse, VECTOR localTop, VECTOR localDown)
	{
		ColliderShapeData data;
		data.shape = ColliderShape::CAPSULE; data.capsule.radius = radiuse; data.capsule.localTop = localTop; data.capsule.localDown = localDown; return data;
	}
	static ColliderShapeData Box(VECTOR halfSize)
	{
		ColliderShapeData data;
		data.shape = ColliderShape::BOX; data.box.halfSize = halfSize; return data;
	}
	VECTOR GetBoundingHalfSize(void)const
	{
		float r = 0;
		float h = 0;
		switch (shape)
		{
		case ColliderShape::BOX:
			r = VSize(box.halfSize);
			return VGet(r, r, r);
			break;
		case ColliderShape::SPHERE:
			return VGet(sphere.radius, sphere.radius, sphere.radius);
			break;
		case ColliderShape::CAPSULE:
			h = VSize(VSub(capsule.localTop, capsule.localDown)) * 0.5f;
			r = capsule.radius + h;
			return VGet(r, r, r);
			break;
		default:
			return VGet(0, 0, 0);
			break;
		}
	}
};

struct ColliderInfo
{
	ColliderLayer layer_ = ColliderLayer::NONE;
	uint32_t mask = 0;
	bool isActive_ = true;
	bool isTrigger_ = false;
	VECTOR offset_ = VGet(0, 0, 0);
	bool isDebug_ = false;
	int debugColor_ = GetColor(255, 0, 0);
};

namespace CollisionFilter {
	inline uint32_t LayerBit(ColliderLayer layer){
		return 1 << static_cast<uint32_t>(layer);
	}
	inline uint32_t MakeMask(std::initializer_list<ColliderLayer> layers){
		uint32_t mask = 0;
		for (auto layer : layers){
			mask |= LayerBit(layer);
		}
		return mask;
	}
	inline bool CanCollide(const ColliderInfo& a, const ColliderInfo& b)
	{
		if (!a.isActive_ || !b.isActive_) return false;
		return (a.mask & LayerBit(b.layer_)) != 0 || (b.mask & LayerBit(a.layer_)) != 0;
	}
}	