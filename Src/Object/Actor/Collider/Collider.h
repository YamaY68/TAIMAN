#pragma once
#include<cstdint>
#include<DxLib.h>
#include<initializer_list>

static constexpr VECTOR ZERO = { 0.0f,0.0f,0.0f };


enum class ColliderLayer :uint8_t
{
	NONE=0,
	STAGE,
	ACTOR,
	PLAYER_ATTACK,
	ENEMY_ATTACK,

	MAX,
};

enum class ColliderShape :uint8_t
{
	NONE=0,
	SPHERE,
	BOX,
	CAPSULE,

	MAX,
};

namespace ColliderFilter
{
	inline uint32_t LayerBit(ColliderLayer layer)
	{
		if (layer == ColliderLayer::NONE)return 0;
		return 1 << static_cast<uint32_t>(layer);
	}

	inline uint32_t MakeMask(std::initializer_list<ColliderLayer>layers)
	{
		uint32_t mask = 0;
		for (auto layer : layers)
		{
			mask |= LayerBit(layer);
		}
		return mask;
	}
	inline bool CheckMask(uint32_t maskA, ColliderLayer layerB, uint32_t maskB, ColliderLayer layerA)
	{
		if (layerA == ColliderLayer::NONE || layerB == ColliderLayer::NONE)return false;
		return (maskA & LayerBit(layerB)) != 0 || (maskB & LayerBit(layerA)) != 0;
	}
}

struct Collider
{
	ColliderLayer layer = ColliderLayer::NONE;
	uint32_t mask = 0;
	bool isActive = true;
	bool isTrigger = false;
	VECTOR offset = ZERO;
	bool isDebug = false;
	int debugColor = GetColor(255, 0, 0);

	const VECTOR* parentPos_ = nullptr;

	ColliderShape shape = ColliderShape::NONE;
	union
	{
		struct { float radius; }sphere;
		struct { VECTOR halfSize; }box;
		struct { float radius; VECTOR localTop; VECTOR localDown; }capsule;
	};

	static Collider CreateSphere(float radius, ColliderLayer layer, uint32_t mask, const VECTOR& offset =ZERO, bool isTrigger = false)
	{
		Collider collider;
		collider.shape = ColliderShape::SPHERE;
		collider.sphere.radius = radius;
		collider.layer = layer;
		collider.mask = mask;
		collider.offset = offset;
		collider.isTrigger = isTrigger;
		return collider;
	}

	static Collider CreateBox(const VECTOR& halfSize, ColliderLayer layer, uint32_t mask, const VECTOR& offset = ZERO, bool isTrigger = false)
	{
		Collider collider;
		collider.shape = ColliderShape::BOX;
		collider.box.halfSize = halfSize;
		collider.layer = layer;
		collider.mask = mask;
		collider.offset = offset;
		collider.isTrigger = isTrigger;
		return collider;
	}

	static Collider CreateCapsule(float radius, const VECTOR& localTop, const VECTOR& localDown, ColliderLayer layer, uint32_t mask, const VECTOR& offset = ZERO, bool isTrigger = false)
	{
		Collider collider;
		collider.shape = ColliderShape::CAPSULE;
		collider.capsule.radius = radius;
		collider.capsule.localTop = localTop;
		collider.capsule.localDown = localDown;
		collider.layer = layer;
		collider.mask = mask;
		collider.offset = offset;
		collider.isTrigger = isTrigger;
		return collider;
	}

	VECTOR GetWorldPos(void)const { return VAdd(*parentPos_, offset); }
	void DrawDebug(void)const
	{
		if (!isDebug || !isActive)return;
		VECTOR pos = GetWorldPos();
		switch (shape)
		{
		case ColliderShape::NONE:
			break;
		case ColliderShape::SPHERE:
			DrawSphere3D(pos, sphere.radius, 16, debugColor, debugColor, false);
			break;
		case ColliderShape::BOX:
			DrawCube3D(VAdd(pos, box.halfSize), VSub(pos, box.halfSize), debugColor, debugColor, false);
			break;
		case ColliderShape::CAPSULE:
			DrawCapsule3D(pos, VAdd(pos, capsule.localTop), capsule.radius, 16, debugColor, debugColor, false);
			break;
		case ColliderShape::MAX:
			break;
		default:
			break;
		}
	}

};