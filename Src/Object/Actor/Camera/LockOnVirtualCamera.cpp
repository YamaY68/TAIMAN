#include "LockOnVirtualCamera.h"

void LockOnVirtualCamera::Update(void)
{
	if (!playerTrans_ || !targetTrans_) return;

	VECTOR pPos = playerTrans_->pos;
	VECTOR tPos = targetTrans_->pos;

	// プレイヤーと敵の中間地点を注視点にする
	VECTOR midTarget = VScale(VAdd(pPos, tPos), 0.5f);
	midTarget.y += 60.0f;
	state_.target = midTarget;

	VECTOR diff = VSub(pPos, tPos);
	float dist = VSize(diff);
	float camDis = 300.0f + dist * 0.4f;

	VECTOR dir = (dist > 0.001f) ? VScale(diff, 1.0f / dist) : VGet(0, 0, -1);
	VECTOR camPos = VAdd(pPos, VScale(dir, camDis));
	camPos.y += 100.0f + dist * 0.15f; 

	if (isFirstFrame_)
	{
		currentCamPos_ = camPos;
		state_.position = currentCamPos_;
		isFirstFrame_ = false;
	}

	currentCamPos_ = VAdd(currentCamPos_, VScale(VSub(camPos, currentCamPos_), 0.1f));
	state_.position = currentCamPos_;

	VECTOR deadZoneOffset = CalculateDeadZoneOffset(VGet(pPos.x, pPos.y + 60.0f, pPos.z));

	currentCamPos_ = VAdd(currentCamPos_, deadZoneOffset);
	state_.position = currentCamPos_;

	state_.target = midTarget;
	state_.upVector = VGet(0.0f, 1.0f, 0.0f);

	// 注視点へ向く回転クォータニオンの算出
	VECTOR dirToTarget = VSub(state_.target, state_.position);
	state_.rotation = Quaternion::LookRotation(dirToTarget);
}