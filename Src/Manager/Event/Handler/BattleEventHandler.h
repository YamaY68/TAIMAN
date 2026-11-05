#pragma once
#include "IEventHandler.h"
class TimeManager;
class CameraManager;

class BattleEventHandler :
    public IEventHandler
{
public:
    BattleEventHandler(TimeManager* timeManager, CameraManager* cameraManager = nullptr)
        :timeManager_(timeManager), cameraManager_(cameraManager) {
    }
    ~BattleEventHandler(void)override { Release(); }

    void Init(GameContext* context)override;
    void Release(void)override;

private:
    TimeManager* timeManager_;
    CameraManager* cameraManager_;

};

