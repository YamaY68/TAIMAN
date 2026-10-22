#include "EventPathVirtualCamera.h"

void EventPathVirtualCamera::Play(VECTOR startPos, VECTOR endPos, VECTOR targetPos, float duration)
{
	startPos_ = startPos;
	endPos_ = endPos;
	targetPos_ = targetPos;
	duration_ = duration;
	elapsedTime_ = 0.0f;
	isPlaying_ = true;
	SetActive(true);
}

void EventPathVirtualCamera::Update(void)
{
	if (!IsPlaying())return;
	constexpr float DELTA_TIME = 1.0f / 60.0f;
	elapsedTime_ += DELTA_TIME;
	float t = elapsedTime_ / duration_;

	if (t >= 1.0f)
	{
		t = 1.0f;
		isPlaying_ = false;
		SetActive(false);
	}

	float smoothT = t * t * (3.0f - 2.0f * t);
	state_.position = VAdd(startPos_, VScale(VSub(endPos_, startPos_), smoothT));
	state_.target = targetPos_;
	state_.upVector = VGet(0.0f, 1.0f, 0.0f);

	VECTOR dirToTarget = VSub(state_.target, state_.position);
	state_.rotation = Quaternion::LookRotation(dirToTarget);
}