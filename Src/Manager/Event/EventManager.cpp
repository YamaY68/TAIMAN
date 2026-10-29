#include "EventManager.h"

EventManager* EventManager::instance_ = nullptr;

void EventManager::CreateInstance(void)
{

 }
EventManager& EventManager::GetInstance(void)
{
	return *instance_;
 }
void EventManager::DeleteInstance(void)
{

 }

void EventManager::Unsubscribe(void* subscriber)
{
	for (auto& [type, list] : handlers_)
	{
		std::erase_if(list, [subscriber](const Handler& h)
		{
			return h.subscriber == subscriber; 
		});
	}
}

void EventManager::ClearAll(void)
{
	handlers_.clear();
}


EventManager::~EventManager(void)
{
}
