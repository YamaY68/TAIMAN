#include "PlayerBase.h"

PlayerBase::PlayerBase(void)
{
}

PlayerBase::~PlayerBase(void)
{
}

void PlayerBase::SubUpdate(void)
{
	auto* curState = GetCurrentState();
	if (curState)
	{
		stateMachine_.HandleInput();
		if (curState->GetStateFrame() >= curState->GetNextInputStartTime())
		{
			curState->OnCanChange();
		}
		curState->IncreaseStateFrame();
	}

	if (comboBufferTimer_ > 0.0f)
	{
		comboBufferTimer_--;
		if (comboBufferTimer_ <= 0.0f)
		{
			ClearCombo();
		}
	}
}

void PlayerBase::SubDraw(void)
{
DrawFormatString(0, 40, 0xffffff, "ComboBuffer: %.0f", comboBufferTimer_);
}

void PlayerBase::SetNextComboChoices(int lightType, int heavyType)
{
	nextLightAttackType_ = lightType;
	nextHeavyAttackType_ = heavyType;
}

void PlayerBase::SetComboBuffer(float frames)
{
	comboBufferTimer_ = frames;
}

void PlayerBase::ClearCombo(void)
{
	nextLightAttackType_ = -1;
	nextHeavyAttackType_ = -1;
	comboBufferTimer_ = 0.0f;
}

void PlayerBase::SetAttackType(bool isLight)
{
	isLightAttack_ = isLight;
}
