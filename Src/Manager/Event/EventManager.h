#pragma once
#include<functional>
#include<vector>
#include<unordered_map>
#include<typeindex>
#include<memory>

class EventManager
{
public:
	static void CreateInstance(void);
	static EventManager& GetInstance(void);
	static void DeleteInstance(void);
	



private:
	static EventManager* instance_;
	EventManager(void);
	EventManager(const EventManager& instance) = default;
	~EventManager(void);
};

