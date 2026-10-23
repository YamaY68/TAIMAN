#pragma once
#include<vector>
#include<memory>
#include<EffekseerForDXLib.h>
#include"VirtualCameraBase.h"

class CameraManager
{
public:
	CameraManager(void);
	~CameraManager(void);

	void Update(void);
	void ApplyDxLib(void);

	void AddCamera(VirtualCameraBase* cam);
	void RemoveCamera(VirtualCameraBase* cam);

	void Shake(float duration, float magnitude);

	const CameraState& GetCurrentState(void) const { return currentState_; }
	float GetCameraYaw(void) const
	{
		VECTOR forward = VSub(currentState_.target, currentState_.position);
		return atan2f(forward.x, forward.z);
	}
	VECTOR GetCameraForwardXZ(void) const
	{
		VECTOR forward = VSub(currentState_.target, currentState_.position);
		forward.y = 0.0f;
		if (VSquareSize(forward) > 0.0001f)
		{
			return VNorm(forward);
		}
		return VGet(0.0f, 0.0f, 1.0f);
	}
private:
	VirtualCameraBase* FindHighestPriorityCamera(void);

	std::vector<VirtualCameraBase*>cameras_;
	VirtualCameraBase* currentCamera_ = nullptr;
	VirtualCameraBase* prevCamera_ = nullptr;

	CameraState currentState_;
	float blendTime = 0.0f;
	float blendDuration_ = 1.0f;	//ƒJƒƒ‰‘JˆÚ‚É‚©‚¯‚é•b”
	bool isBlending_ = false;

	float shakeTime_ = 0.0f;
	float shakeDuration_ = 0.0f;
	float shakeMagnitude_ = 0.0f;
	VECTOR shakeOffset_ = { 0.0f,0.0f,0.0f };

};

