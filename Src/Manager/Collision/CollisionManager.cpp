#include "CollisionManager.h"
#include<cmath>
#include<algorithm>
#include"../../Object/Manager/ActorManager/ActorManager.h"
#include"../../Object/Actor/ActorBase.h"
#include"CollisionLogic.h"

CollisionManager::CollisionManager(void)
{
	entries_.reserve(ActorManager::MAX_ACTORS);
}

CollisionManager::~CollisionManager(void)
{
	entries_.clear();
}

void CollisionManager::RegisterActor(ActorBase* actor, uint32_t ownerID)
{
	if (!actor)return;
	auto& colliders = actor->GetColliders();
	for (uint32_t i = 0; i < colliders.size(); ++i)
	{
		colliders[i].parentPos_ = &actor->GetTransform().pos;
		entries_.push_back({ ownerID,actor,&colliders[i],i });
	}
}

void CollisionManager::UnregisterActor(uint32_t ownerID)
{
	std::erase_if(entries_, [ownerID](const ColliderEntry& e)
		{
			return e.ownerID == ownerID;
		});
}

void CollisionManager::Clear(void)
{
	entries_.clear();
}


void CollisionManager::Update(void)
{
	std::vector<ContactRecord>contacts;
	const size_t entryCount = entries_.size();
	for (int iter = 0; iter < SOLVE_ITERATIONS; ++iter)
	{
		bool anyHit = false;
		for (size_t i = 0; i < entryCount; ++i)
		{
			for (size_t j = i + 1; j < entryCount; ++j)
			{
				bool isTriggerPair = false;
				if (!CanCollide(entries_[i], entries_[j], iter, isTriggerPair))continue;
				if (ProcessPair(i, j, iter, isTriggerPair, contacts))anyHit = true;
			}
		}
		if (iter > 0 && !anyHit)break;
	}
	DispatchHitEvents(contacts);
}

void CollisionManager::Resolve(ActorBase* a, ActorBase* b, const VECTOR& normal, float penetration)
{
	if (!a || !b)return;
	bool aDynamic = a->GetBodyType() == ActorBase::BodyType::DYNAMIC;
	bool bDynamic = b->GetBodyType() == ActorBase::BodyType::DYNAMIC;
	if (!aDynamic && !bDynamic)return;
	
	VECTOR& posA = a->GetTransform().pos;
	VECTOR& posB = b->GetTransform().pos;
	
	if (aDynamic && !bDynamic)
	{
		posA = VAdd(posA, VScale(normal, penetration));
	}
	else if (!aDynamic && bDynamic)
	{
		posB = VAdd(posB, VScale(normal, -penetration));
	}
	else
	{
		const float halfPen = penetration * 0.5f;
		posA = VAdd(posA, VScale(normal, halfPen));
		posB = VAdd(posB, VScale(normal, -halfPen));
	}
}

bool CollisionManager::CanCollide(const ColliderEntry& a, const ColliderEntry& b, int iter, bool& outIsTriggerPair) const
{
	if (!a.collider->isActive || !b.collider->isActive)return false;
	if (a.ownerID == b.ownerID)return false;
	outIsTriggerPair = a.collider->isTrigger || b.collider->isTrigger;
	if (iter > 0 && outIsTriggerPair)return false;
	if (!ColliderFilter::CheckMask(a.collider->mask, b.collider->layer, b.collider->mask, a.collider->layer))return false;
	return true;
}

bool CollisionManager::ProcessPair(size_t indexA, size_t indexB, int iter, bool isTriggerPair, std::vector<ContactRecord>& outContacts)
{
	auto& a = entries_[indexA];
	auto& b = entries_[indexB];
	CollisionPairType pair = CollisionLogic::GetCollisionPairType(a.collider->shape, b.collider->shape);
	CollisionResult result = CollisionLogic::DispatchCollision(pair, a.collider, b.collider);

	if (!result.isHit) return false;

	if (isTriggerPair)
	{
		outContacts.push_back(ContactRecord{
			.indexA = indexA,
			.indexB = indexB,
			.hitPoint = result.hitPoint,
			.isTrigger = true, });
		return false;
	}
	if (result.penetration > 0.001f)
	{
		Resolve(a.ownerActor, b.ownerActor, result.normal, result.penetration);

		// 通知リストへの登録は初回イテレーション（iter == 0）のみ
		if (iter == 0)
		{
			outContacts.push_back({ indexA, indexB, result.hitPoint, false });
		}
		return true;
	}

	return false;

}

void CollisionManager::DispatchHitEvents(const std::vector<ContactRecord>& contacts)
{
	for (const auto& c : contacts)
	{
		auto& a = entries_[c.indexA];
		auto& b = entries_[c.indexB];

		HitContext hitA{
			.otherActor = b.ownerActor,
			.otherCollider = b.collider,
			.otherSubIndex = b.subIndex,
			.isTrigger = c.isTrigger,
			.hitPoint = c.hitPoint,
		};
		HitContext hitB{
			.otherActor = a.ownerActor,
			.otherCollider = a.collider,
			.otherSubIndex = a.subIndex,
			.isTrigger = c.isTrigger,
			.hitPoint = c.hitPoint,
		};

		a.ownerActor->OnCollision(hitA);
		b.ownerActor->OnCollision(hitB);
	}
}

	