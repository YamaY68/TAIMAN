#include "EventManager.h"

EventManager* EventManager::instance_ = nullptr;

EventManager::EventManager(void)
{

}

EventManager::~EventManager(void)
{
	ClearAll();
}
void EventManager::CreateInstance(void)
{
	if (!instance_)
	{
		instance_ = new EventManager();
	}
 }
EventManager& EventManager::GetInstance(void)
{
	return *instance_;
 }
void EventManager::DeleteInstance(void)
{
	if (instance_)
	{
		instance_->ClearAll();
		delete instance_;
		instance_ = nullptr;
	}
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


