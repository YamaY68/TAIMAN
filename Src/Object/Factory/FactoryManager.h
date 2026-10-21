#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include"FactoryBase.h"

class ActorBase;
enum class FACTORY_TYPE:uint8_t
{
	NONE = 0,
	PLAYER,
	ENEMY,
	STAGE,
};

class FactoryManager
{
public:
	FactoryManager(void);
	~FactoryManager(void);

	void RegisterFactory(FACTORY_TYPE type, std::unique_ptr<FactoryBase<ActorBase>>factory)
	{
		factories_[type]=std::move(factory);
	}

	std::vector<std::unique_ptr<ActorBase>>Create(FACTORY_TYPE type);

	void Clear(void) { factories_.clear(); }
private:
	std::unordered_map<FACTORY_TYPE, std::unique_ptr<FactoryBase<ActorBase>>>factories_;
};

