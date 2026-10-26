#pragma once
#include<typeindex>

template <typename T>

class IState
{
public:
	virtual ~IState(void) {};

	virtual void Enter(T* owner) = 0;
	virtual void Update(T* owner) = 0;
	virtual void Exit(T* owner) = 0;

	virtual void HandleInput(T* owner) {};

	virtual const char* GetName(void)const { return typeid(*this).name(); }

	void IncreaseStateFrame(void) { stateFrame_++; }
	int GetStateFrame(void)const { return stateFrame_; }

	int GetNextInputStartTime(void)const { return nextInputStartTime_; }
	bool CanChange(void)const { return canChange_; }
	void OnCanChange(void) { canChange_ = true; }

	bool GetIsLoop(void)const { return isLoop_; }

protected:
	int stateFrame_ = 0;
	int nextInputStartTime_ = 0;
	int totalAnimFrame_ = 0;
	bool canChange_ = false;
	bool isLoop_ = false;

};
