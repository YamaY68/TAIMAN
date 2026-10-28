#pragma once
#include<string>
#include"CameraState.h"

class VirtualCameraBase
{
public	:

	VirtualCameraBase(int priority = 10) :priority_(priority), isActive_(true) {}
	virtual ~VirtualCameraBase(void)=default;

	virtual void Update(void) = 0;

	const CameraState& GetState(void)const { return state_; }
	int GetPriority(void)const {return priority_;}

	void SetPriority(int priority) { priority_ = priority; }
	bool IsActive(void)const {return isActive_;}
	void SetActive(bool active) { isActive_ = active; }
	void SetCameraKind(CAMERA_KIND kind) { state_.kind_ = kind; }
protected:
	CameraState state_;
	int priority_;
	bool isActive_=false;
};

