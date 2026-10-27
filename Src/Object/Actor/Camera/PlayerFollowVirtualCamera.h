#pragma once
#include "VirtualCameraBase.h"
#include"../../Common/Transform.h"
#include"../../../Manager/Input/KeyManager.h"
#include"../../../Common/Quaternion.h"

class PlayerFollowVirtualCamera :
    public VirtualCameraBase
{
public:
		PlayerFollowVirtualCamera(const Transform* playerTrans, int priority = 10)
		: VirtualCameraBase(priority), playerTrans_(playerTrans)
	{
		distance_ = 200.0f;
		height_ = 60.0f;
		angles_ = { 15.0f * (DX_PI_F / 180.0f), 0.0f, 0.0f };
	}

		void Update(void)override;

private:
	void ProcessRotation(void);
	VECTOR ResolveWallCollision(const VECTOR& target, const VECTOR& idealPos);

	const Transform* playerTrans_;
	VECTOR angles_;
	float distance_;
	float height_;
	int stageModelId_ = -1;
};

