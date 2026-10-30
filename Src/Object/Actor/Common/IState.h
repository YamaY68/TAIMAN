#pragma once
#include<typeindex>

template <typename T>

class IState
{
public:
	//デストラクタ
	virtual ~IState(void) {};
	//進入
	virtual void Enter(T* owner) = 0;
	//更新
	virtual void Update(T* owner) = 0;
	//離脱
	virtual void Exit(T* owner) = 0;
	//入力受付処理
	virtual void HandleInput(T* owner) {};
	//ステート型名の取得
	virtual const char* GetName(void)const { return typeid(*this).name(); }
	//経過フレームの加算
	void IncreaseStateFrame(void) { stateFrame_++; }
	//経過フレームの取得
	int GetStateFrame(void)const { return stateFrame_; }
	//次入力受付フレームの取得
	int GetNextInputStartTime(void)const { return nextInputStartTime_; }
	//遷移可能か
	bool CanChange(void)const { return canChange_; }
	//遷移可能フラグを有効か
	void OnCanChange(void) { canChange_ = true; }
	//ループするかどうか
	bool GetIsLoop(void)const { return isLoop_; }

protected:
	int stateFrame_ = 0;
	int nextInputStartTime_ = 0;
	int totalAnimFrame_ = 0;
	bool canChange_ = false;
	bool isLoop_ = false;

};
