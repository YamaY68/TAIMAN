#pragma once
#include<memory>
#include<vector>

template<typename T>

class FactoryBase
{
public:
	FactoryBase(void) = default;
	virtual ~FactoryBase(void)= default;
	virtual std::vector<std::unique_ptr<T>>Create(void) = 0;
};