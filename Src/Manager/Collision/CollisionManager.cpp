#include "CollisionManager.h"
#include<cmath>
#include<algorithm>
#include"../../Object/Manager/ActorManager/ActorManager.h"
#include"../../Object/Actor/ActorBase.h"

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
	std::vector<std::pair<size_t, size_t>> collisionPairs;

	for (int iter = 0; iter < 3; ++iter)
	{
		
	}
}

