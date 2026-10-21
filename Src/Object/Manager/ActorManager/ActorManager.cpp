#include "ActorManager.h"
#include"../../../Manager/Collision/CollisionManager.h"
#include"../../Actor/ActorBase.h"


ActorManager::ActorManager(void)
{
	actors_.resize(MAX_ACTORS);
	freeEntityIDs_.resize(MAX_ACTORS);
	pendingDestroys_.resize(256);
	for (size_t i = 0; i < MAX_ACTORS; ++i)
	{
		freeEntityIDs_.push_back(static_cast<EntityID>(MAX_ACTORS - 1 - i));
	}
}


void ActorManager::Load(void)
{
	for (auto& actor : actors_)
	{
		actor->Load();
	}
}

void ActorManager::Init(void)
{
	for (size_t i = 0; i < MAX_ACTORS; ++i)
	{
		if (!actors_[i])continue;
		actors_[i]->Init();
		if (!colMng_)continue;
		colMng_->RegisterActor(actors_[i].get(), static_cast<uint32_t>(i));
	}
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
	for (size_t i = 0; i < MAX_ACTORS; ++i)
	{
		if (!actors_[i])continue;
		if (colMng_)
		{
			colMng_->UnregisterActor(static_cast<uint32_t>(i));
		}
		actors_[i]->Release();
		actors_[i].reset();
	}
	freeEntityIDs_.clear();
	for (size_t i = 0; i < MAX_ACTORS; ++i)
	{
		freeEntityIDs_.push_back(static_cast<EntityID>(MAX_ACTORS - 1 - i));
	}
	pendingDestroys_.clear();
	activeEntityCount_ = 0;
	}

std::vector<EntityID> ActorManager::RegisterActors(std::vector<std::unique_ptr<ActorBase>> newActors)
{
	std::vector<EntityID>registeredIDs;
	registeredIDs.reserve(newActors.size());

	for (auto& actor : newActors)
	{
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
