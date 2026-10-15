#pragma once
#include<memory>
#include<vector>

class ActorBase;

class FactoryBase
{
public:
	FactoryBase(void) = default;
	virtual ~FactoryBase(void)= default;
	virtual std::vector<std::unique_ptr<ActorBase>>CreateActor(void) = 0;
};