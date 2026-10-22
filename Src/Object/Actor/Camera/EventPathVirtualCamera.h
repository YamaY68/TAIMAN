#pragma once
#include "VirtualCameraBase.h"
class EventPathVirtualCamera :
    public VirtualCameraBase
{
public:
    EventPathVirtualCamera(int priority = 25) :VirtualCameraBase(priority) {}

    void Play(VECTOR startPos, VECTOR endPos, VECTOR targetPos, float duration);
    void Update(void)override;

    bool IsPlaying(void)const { return isPlaying_; }

private:
    VECTOR startPos_ = VGet(0, 0, 0);
    VECTOR endPos_ = VGet(0, 0, 0);
    VECTOR targetPos_ = VGet(0, 0, 0);
    float duration_ = 2.0f;
    float elapsedTime_ = 0.0f;
    bool isPlaying_ = false;
};

