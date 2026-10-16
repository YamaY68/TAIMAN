#pragma once
#include<vector>
#include<memory>
#include<cstdint>
#include<unordered_map>

class ActorBase;
using EntityID = uint32_t;

class ActorManager
{
	static constexpr size_t MAX_ACTORS = 10000;

	ActorManager(void);
	~ActorManager(void)=default;

	void Load(void);
	void Init(void);
	void Update(void);
	void PostUpdate(void);
	void Draw(void);
	void Release(void);

	const std::vector<std::unique_ptr<ActorBase>>& GetActors(void)const { return actors_ }
	size_t GetActiveActorCount(void)const { return activeEntityID; }

	ActorBase* GetActor(EntityID id)
	{
		if (id < MAX_ACTORS && actors_[id])return actors_[id].get();
		return nullptr;
	}

private:
	std::vector<std::unique_ptr<ActorBase>>actors_;
	std::vector<EntityID>freeEntityIDs_;
	EntityID activeEntityID = 0;
	std::vector<EntityID>pedingDestroys_;
};

