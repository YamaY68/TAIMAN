#include "Movement.h"

Movement::Movement(void)
	:
	inputMovePow_(ZERO),
	movePow_(ZERO),
	externalPow_(ZERO),
	isGrounded_(false),
	useGravity_(true)
{
}

Movement::~Movement(void)
{
}

void Movement::Update(float timeScale)
{
	if (useGravity_ && !isGrounded_)
	{
		fallSpeed_ += GRAVITY;
	}
	else if (isGrounded_)
	{
		fallSpeed_ = 0;
	}
	externalPow_ = VScale(externalPow_, DECAY);
	if (VSquareSize(externalPow_) < 0.01f)
	{
		externalPow_ = ZERO;
	}
	movePow_ = VAdd(inputMovePow_, externalPow_);
	movePow_.y -= fallSpeed_;
	movePow_ = VScale(movePow_, timeScale);
	inputMovePow_ = ZERO;
	isGrounded_ = false;
}
