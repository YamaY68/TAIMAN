#include "LockOnVirtualCamera.h"

void LockOnVirtualCamera::Update(void)
{
	if (!playerTrans_ || !targetTrans_) return;

	VECTOR pPos = playerTrans_->pos;
	VECTOR tPos = targetTrans_->pos;

	// プレイヤーと敵の中間地点を注視点にする
	VECTOR midTarget = VScale(VAdd(pPos, tPos), 0.5f);
	midTarget.y += 60.0f;

	VECTOR diff = VSub(pPos, tPos);
	float dist = VSize(diff);

	float camDis = 300.0f + dist * 0.4f;

	VECTOR dir = (dist > 0.001f) ? VScale(diff, 1.0f / dist) : VGet(0, 0, -1);
	VECTOR camPos = VAdd(pPos, VScale(dir, camDis));
	camPos.y += 100.0f + dist * 0.15f; // 修正: + ではなく *

	state_.position = camPos;
	state_.target = midTarget;
	state_.upVector = VGet(0.0f, 1.0f, 0.0f);

	// 注視点へ向く回転クォータニオンの算出
	VECTOR dirToTarget = VSub(state_.target, state_.position);
	state_.rotation = Quaternion::LookRotation(dirToTarget);
}