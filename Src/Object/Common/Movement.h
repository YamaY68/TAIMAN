#pragma once
#include<DxLib.h>

class Movement
{
public:
	static constexpr VECTOR ZERO = { 0.0f,0.0f,0.0f };
	static constexpr float DECAY = 0.85f;
	static constexpr float GRAVITY = 9.8 / 60;

	Movement(void);
	~Movement(void);

	void Update(float timeScale=1.0f);

	void SetInputMovePow(VECTOR pow) { inputMovePow_ = pow; }
	void AddExternalPow(VECTOR pow) { externalPow_ = VAdd(externalPow_,pow); }
	void SetIsGrounded(bool flag) { isGrounded_ = flag; }
	void SetUseGravity(bool flag) { useGravity_ = flag; }

	bool IsGrounded(void) { return isGrounded_; }
	VECTOR GetMovePow(void) { return movePow_; }
	void ResetFallSpeed(void) { fallSpeed_ = 0.0f; }
private:
	VECTOR inputMovePow_;
	VECTOR movePow_;
	VECTOR externalPow_;
	float fallSpeed_;
	bool isGrounded_;
	bool useGravity_;
};

