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
	
	template <typename EventType>
	void Subscribe(void* subscriber, std::function<void(const EventType&)>callback);
	void Unsubscribe(void* subscriber);
	template<typename EventType>
	void Publish(const EventType& event);
	void ClearAll(void);


private:
	static EventManager* instance_;
	EventManager(void);
	EventManager(const EventManager& instance) = default;
	EventManager& operator=(const EventManager&) = default;
	~EventManager(void);

	struct Handler
	{
		void* subscriber;
		std::function<void(const void*)>callback;
	};
	std::unordered_map<std::type_index, std::vector<Handler>>handlers_;

};

template<typename EventType>
inline void EventManager::Subscribe(void* subscriber, std::function<void(const EventType&)> callback)
{
	auto type = std::type_index(typeid(EventType));
	handlers_[type].push_back({
		subscriber,
		[callback](const void* data) {
			callback(*static_cast<const EventType*>(data)); 
		} 
	});
}

template<typename EventType>
inline void EventManager::Publish(const EventType& event)
{
	auto it = handlers_.find(std::type_index(typeid(EventType)));
	if (it != handlers_.end())
	{
		auto list = it->second;
		for (const auto& h : list)
		{
			h.callback(&event);
		}
	}
}
