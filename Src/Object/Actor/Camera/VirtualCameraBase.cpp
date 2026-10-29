#include "VirtualCameraBase.h"
#include"../../../Application.h"
VECTOR VirtualCameraBase::CalculateDeadZoneOffset(const VECTOR& targetWorldPos) const
{
	VECTOR toTarget = VSub(targetWorldPos, state_.position);
	VECTOR forward = VNorm(VSub(state_.target, state_.position));

    float forwardDist = VDot(toTarget, forward);
    if (forwardDist <= 1.0f)return VGet(0.0f, 0.0f, 0.0f);

    VECTOR horizontalForward = forward;
    horizontalForward.y = 0.0f;
    if (VSize(horizontalForward) < 0.001f)horizontalForward = VGet(0.0f, 0.0f, 1.0f);
    horizontalForward = VNorm(horizontalForward);
    VECTOR worldUp = VGet(0.0f, 1.0f, 0.0f);
    VECTOR camRightHorizontal = VNorm(VCross(worldUp, horizontalForward));
    float localX = VDot(toTarget, camRightHorizontal);
    float halfFovTan = 0.577f;
    float maxAllowedX = forwardDist * halfFovTan * deadZoneRatioX_;
    float excessX = 0.0f;
    if (localX > maxAllowedX)
    {
        excessX = localX - maxAllowedX; // 右にはみ出た
    }
    else if (localX < -maxAllowedX)
    {
        excessX = localX - (-maxAllowedX); // 左にはみ出た（負値）
    }

    if (excessX == 0.0f)
    {
        return VGet(0.0f, 0.0f, 0.0f);
    }

    // 水平の横方向にはみ出し量分だけオフセットを返す
    return VScale(camRightHorizontal, excessX * deadZoneFollowSpeed_);
}