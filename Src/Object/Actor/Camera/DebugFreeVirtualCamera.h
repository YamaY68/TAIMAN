#pragma once
#include "VirtualCameraBase.h"
#include"../../../Manager/Input/KeyManager.h"

class DebugFreeVirtualCamera :
    public VirtualCameraBase
{
public:
    DebugFreeVirtualCamera(int priority = 999) :VirtualCameraBase(priority)
    {
        isActive_ = false;
        speed_ = 10.0f;
    }

    void Update(void)override;

    void SetSpeed(float speed) { speed_ = speed; }

private:
    float speed_;
    float yaw_ = 0.0f;
    float pitch_ = 0.0f;
};

