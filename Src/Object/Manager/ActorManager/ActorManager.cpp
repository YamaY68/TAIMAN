#include "ActorManager.h"
#include"../../../Manager/Collision/CollisionManager.h"
#include"../../Actor/ActorBase.h"


ActorManager::ActorManager(void)
{
	actors_.resize(MAX_ACTORS);
	freeEntityIDs_.clear();
	freeEntityIDs_.reserve(MAX_ACTORS);
	for (size_t i = 0; i < MAX_ACTORS; ++i)
	{
		freeEntityIDs_.push_back(static_cast<EntityID>(MAX_ACTORS - 1 - i));
	}
}

ActorManager::~ActorManager(void)
{
}

void ActorManager::Update(void)
{
	for (auto& actor : actors_)
	{
		if (actor)
		{
			actor->Update();
		}
	}
	}
void ActorManager::PostUpdate(void)
{
	for (auto& actor : actors_)
	{
		if (actor)
		{
			actor->PostUpdate();
		}
	}
	ProcessPendingDestroys();
	}
void ActorManager::Draw(void)
{
	for (auto& actor : actors_)
	{
		if (actor)
		{
			actor->Draw();
		}
	}
	}
void ActorManager::Release(void)
{
	freeEntityIDs_.clear();
	freeEntityIDs_.reserve(MAX_ACTORS);

	for (size_t i = 0; i < MAX_ACTORS; ++i)
	{
		if (actors_[i])
		{
			if (colMng_)
			{
				colMng_->UnregisterActor(static_cast<uint32_t>(i));
			}
			actors_[i]->Release();
			actors_[i].reset();
		}
		freeEntityIDs_.push_back(static_cast<EntityID>(MAX_ACTORS - 1 - i));
	}

	pendingDestroys_.clear();
	activeEntityCount_ = 0;
}

std::vector<EntityID> ActorManager::RegisterActors(std::vector<std::unique_ptr<ActorBase>> newActors)
{
	std::vector<EntityID> registeredIDs;
	registeredIDs.reserve(newActors.size());

	for (auto& actor : newActors)
	{
		if (!actor) continue;
		EntityID id = RegisterActor(std::move(actor));
		if (id != INAVALID_ENTITY_ID)
		{
			registeredIDs.push_back(id);
		}
	}
	return registeredIDs;
}

EntityID ActorManager::RegisterActor(std::unique_ptr<ActorBase> newActor)
{
	if (!newActor || freeEntityIDs_.empty())return INAVALID_ENTITY_ID;
	EntityID id = freeEntityIDs_.back();
	freeEntityIDs_.pop_back();
	actors_[id] = std::move(newActor);
	actors_[id]->Load();
	actors_[id]->Init();
	if (colMng_)colMng_->RegisterActor(actors_[id].get(), id);
	++activeEntityCount_;
	return id;
}

void ActorManager::DestroyActor(EntityID id)
{
	if (id < MAX_ACTORS && actors_[id])
	{
		if (std::find(pendingDestroys_.begin(), pendingDestroys_.end(), id) == pendingDestroys_.end())
		{
			pendingDestroys_.push_back(id);
		}
	}
}

ActorBase* ActorManager::FindActorByKind(ENTITY_KIND kind) const
{
	for (const auto& actor : actors_)
	{
		// 生存しており、Kindが一致する最初のアクターを返す
		if (actor && actor->GetEntityKind() == kind)
		{
			return actor.get();
		}
	}
	return nullptr;
}

std::vector<ActorBase*> ActorManager::FindActorsByKind(ENTITY_KIND kind) const
{
	std::vector<ActorBase*> result;
	if (activeEntityCount_ > 0)
	{
		result.reserve(activeEntityCount_); 
	}

	for (const auto& actor : actors_)
	{
		if (actor && actor->GetEntityKind() == kind)
		{
			result.push_back(actor.get());
		}
	}
	return result;
}

void ActorManager::ProcessPendingDestroys(void)
{
	for (EntityID id : pendingDestroys_)
	{
		if (actors_[id])
		{
			if (colMng_)
			{
				colMng_->UnregisterActor(id);
			}
			actors_[id]->Release();
			actors_[id].reset();

			freeEntityIDs_.push_back(id);
			--activeEntityCount_;
		}
	}
	pendingDestroys_.clear();
}
