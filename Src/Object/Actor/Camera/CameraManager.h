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

