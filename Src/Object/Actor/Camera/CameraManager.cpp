#include "CameraManager.h"
#include<cstdint>
CameraManager::CameraManager(void)
{
}

CameraManager::~CameraManager(void)
{
}

void CameraManager::ApplyDxLib(void)
{
    VECTOR finalPos = VAdd(currentState_.position, shakeOffset_);
    VECTOR finalTarget = VAdd(currentState_.target, shakeOffset_);

    SetCameraNearFar(currentState_.nearClip, currentState_.farClip);
    SetupCamera_Perspective(currentState_.fovRad);

    VECTOR upVector = currentState_.rotation.GetUp();

    SetCameraPositionAndTargetAndUpVec(
        finalPos,
        finalTarget,
        upVector
    );
    Effekseer_Sync3DSetting();
}

void CameraManager::AddCamera(VirtualCameraBase* cam)
{
    cameras_.push_back(cam);
}

void CameraManager::RemoveCamera(VirtualCameraBase* cam)
{
    std::erase(cameras_, cam);
    if (currentCamera_ == cam) currentCamera_ = nullptr;
    if (prevCamera_ == cam) prevCamera_ = nullptr;
}

void CameraManager::Update(void)
{
    constexpr float DELTA_TIME = 1.0f / 60.0f;

    for (auto* cam : cameras_)
    {
        if (cam && cam->IsActive())
        {
            cam->Update();
        }
    }

    VirtualCameraBase* targetCam = FindHighestPriorityCamera();
    if (!targetCam)return;

    if (targetCam != currentCamera_)
    {
        if (currentCamera_ != nullptr)
        {
            prevCamera_ = currentCamera_;
            isBlending_ = true;
            blendTime = 0.0f;
        }
        currentCamera_ = targetCam;
    }

    if (isBlending_ && prevCamera_)
    {
        blendTime+=DELTA_TIME;
        float t = blendTime / blendDuration_;
        if (t >= 1.0f)
        {
            t = 1.0f;
            isBlending_ = false;
            prevCamera_ = nullptr;
        }
        float smoothT = t * t * (3.0f - 2.0f * t);
        currentState_ = CameraState::Lerp(prevCamera_->GetState(), currentCamera_->GetState(), smoothT);
    }
    else
    {
        currentState_ = currentCamera_->GetState();
    }

    shakeOffset_ = { 0.0f,0.0f,0.0f };
    if (shakeTime_ < shakeDuration_)
    {
        shakeTime_ += DELTA_TIME;
        float progress = 1.0f - (shakeTime_ / shakeDuration_);
        float currentMag = shakeMagnitude_ * progress;

        // ランダムな微小オフセット
        shakeOffset_.x = ((rand() % 200 - 100) / 100.0f) * currentMag;
        shakeOffset_.y = ((rand() % 200 - 100) / 100.0f) * currentMag;
        shakeOffset_.z = ((rand() % 200 - 100) / 100.0f) * currentMag;
    }
}

void CameraManager::Shake(float duration, float magnitude)
{
    shakeDuration_ = duration;
    shakeTime_ = 0.0f;
    shakeMagnitude_ = magnitude;
}

VirtualCameraBase* CameraManager::FindHighestPriorityCamera(void)
{
    VirtualCameraBase* best = nullptr;
    int highestPriority = -99999;

    for (auto* cam : cameras_)
    {
        if (cam && cam->IsActive())
        {
            if (cam->GetPriority() > highestPriority)
            {
                highestPriority = cam->GetPriority();
                best = cam;
            }
        }
    }
    return best;
}
