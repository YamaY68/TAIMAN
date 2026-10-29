#pragma once
#include "VirtualCameraBase.h"
#include"../../Common/Transform.h"

class LockOnVirtualCamera :
    public VirtualCameraBase
{
public:
	LockOnVirtualCamera(const Transform* player, const Transform* target, int priority = 15)
		: VirtualCameraBase(priority), playerTrans_(player), targetTrans_(target) {
	}
	void Update(void)override;

	void SetTarget(const Transform* target) { targetTrans_ = target; }
private:
	const Transform* playerTrans_;
	const Transform* targetTrans_;
	VECTOR currentCamPos_ = { 0.0f,0.0f,0.0f };
	bool isFirstFrame_ = true;
};

