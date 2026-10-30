#pragma once
#include<DxLib.h>

class Movement
{
public:
	static constexpr VECTOR ZERO = { 0.0f,0.0f,0.0f };
	static constexpr float DECAY = 0.85f;
	static constexpr float GRAVITY = 9.8 / 60;
	//コンストラクタ
	Movement(void);
	//デストラクタ
	~Movement(void);
	//更新
	void Update(float timeScale=1.0f);
	//入力移動力の設定
	void SetInputMovePow(VECTOR pow) { inputMovePow_ = pow; }
	//外部による推進力の設定
	void AddExternalPow(VECTOR pow) { externalPow_ = VAdd(externalPow_,pow); }
	//接地フラグの設定
	void SetIsGrounded(bool flag) { isGrounded_ = flag; }
	//重力使用フラグの設定
	void SetUseGravity(bool flag) { useGravity_ = flag; }
	//外部推進力の上書き設定
	void SetExternalPow(VECTOR pow) { externalPow_ = pow;}
	//接地状態の取得
	bool IsGrounded(void) { return isGrounded_; }
	//移動量の取得
	VECTOR GetMovePow(void) { return movePow_; }
	//落下速度リセット
	void ResetFallSpeed(void) { fallSpeed_ = 0.0f; }
	//衝突補正
	void OnCollision(const VECTOR& normal);
private:
	VECTOR inputMovePow_;
	VECTOR movePow_;
	VECTOR externalPow_;
	float fallSpeed_;
	bool isGrounded_;
	bool useGravity_;
};

