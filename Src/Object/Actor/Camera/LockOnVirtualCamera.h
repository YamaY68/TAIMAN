#pragma once
#include "VirtualCameraBase.h"
#include"../../Common/Transform.h"
using EntityID = uint32_t; 
class LockOnVirtualCamera :
    public VirtualCameraBase
{
public:
	LockOnVirtualCamera(const Transform* player, const Transform* target, int priority = 15)
		: VirtualCameraBase(priority), playerTrans_(player), targetTrans_(target) {
	}
	void Update(void)override;

	void SetTarget(const Transform* target, EntityID targetId)
	{
		targetTrans_ = target;
		targetId_ = targetId;
		isFirstFrame_ = true;
		SetActive(true);
	}
	void ClearTarget(void) { targetTrans_ = nullptr; targetId_ = 0; SetActive(false); }
	EntityID GetCurrentTargetID(void)const { return targetId_; }
	bool HasTarget(void)const { return targetTrans_ != nullptr; }
private:
	const Transform* playerTrans_=nullptr;
	const Transform* targetTrans_=nullptr;
	EntityID targetId_ = 0;
	VECTOR currentCamPos_ = { 0.0f,0.0f,0.0f };
	bool isFirstFrame_ = true;
};

