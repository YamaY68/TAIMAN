#pragma once
#include "IEventHandler.h"

class ActorManager;
class CameraManager;

class CameraEventHandler :
    public IEventHandler
{
public:
    void Init(GameContext* context)override;
    void Release(void)override;

private:
    ActorManager* actorManager_ = nullptr;
    CameraManager* cameraManager_ = nullptr;
};

