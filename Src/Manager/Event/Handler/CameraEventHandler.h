#pragma once
#include "IEventHandler.h"

class ActorManager;
class CameraManager;

class CameraEventHandler :
    public IEventHandler
{
public:
    CameraEventHandler(ActorManager* actorManager, CameraManager* cameraManager = nullptr)
    : actorManager_(actorManager), cameraManager_(cameraManager) {
    }
    ~CameraEventHandler(void)override { Release(); }

    void Init(GameContext* context)override;
    void Release(void)override;

private:
    ActorManager* actorManager_ = nullptr;
    CameraManager* cameraManager_ = nullptr;
};

