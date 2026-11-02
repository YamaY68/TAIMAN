#pragma once

class GameContext;

class IEventHandler
{
public:
	virtual ~IEventHandler(void) = default;

	virtual void Init(GameContext* context) = 0;
	virtual void Update(void) {};
	virtual void Release(void) {};
};