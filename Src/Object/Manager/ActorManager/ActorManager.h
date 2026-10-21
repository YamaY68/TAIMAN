#pragma once
#include<vector>
#include<memory>
#include<cstdint>
#include<unordered_map>

class ActorBase;
class FactoryBase;
class CollisionManager;
using EntityID = uint32_t;
constexpr EntityID INVALID_ENTITY_ID = 0xffffffff;

class ActorManager
{
public:
	static constexpr size_t MAX_ACTORS = 10000;

	ActorManager(void);
	~ActorManager(void)=default;

	void Load(void);
	void Init(void);
	void Update(void);
	void PostUpdate(void);
	void Draw(void);
	void Release(void);

	void SetCollisionManager(CollisionManager* colMng) { colMng_ = colMng; }
	std::vector<EntityID> RegisterActors(std::vector<std::unique_ptr<ActorBase>>newActors);
	EntityID RegisterActor(std::unique_ptr<ActorBase>newActor);
	void DestroyActor(EntityID id);

	const std::vector<std::unique_ptr<ActorBase>>& GetActors(void)const { return actors_; }
	size_t GetActiveActorCount(void)const { return activeEntityCount_; }

	ActorBase* GetActor(EntityID id)
	{
		if (id < MAX_ACTORS && actors_[id])return actors_[id].get();
		return nullptr;
	}

private:
	void ProcessPendingDestroys(void);

	std::vector<std::unique_ptr<ActorBase>>actors_;
	std::vector<EntityID>freeEntityIDs_;
	std::vector<EntityID>pendingDestroys_;
	size_t activeEntityCount_ = 0;

	CollisionManager* colMng_ = nullptr;
};

