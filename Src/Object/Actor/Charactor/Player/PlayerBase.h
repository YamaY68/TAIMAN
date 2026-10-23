#pragma once
#include "../../ActorBase.h"

class PlayerBase :
    public ActorBase
{
public:
    enum class ANIM_TYPE
    {
		NONE = -1,
		IDLE,
		WALK,
		RUN,
		GUARD,
		EVADE,
		PARRY,

		BACK_EVADE,
		LEFT_STRAFE_WALK,
		LEFT_STRAFE_RUN,
		RIGHT_STRAFE_WALK,
		RIGHT_STRAFE_RUN,

		LIGHT_ATTACK_1,
		LIGHT_ATTACK_2,
		LIGHT_ATTACK_3,
		LIGHT_ATTACK_4,

		HEAVY_ATTACK_1,
		HEAVY_ATTACK_2,
		HEAVY_ATTACK_3,

		FINISH,
    };

    PlayerBase(void);
    virtual ~PlayerBase(void)override;

    void SubUpdate(void)override;
    void SubDraw(void)override;

	virtual void RequestAttack(void) = 0;

	void SetNextComboChoices(int lightType, int heavyType);
	void SetComboBuffer(float frames);
	void ClearCombo(void);
	void SetAttackType(bool isLight);


private:
	virtual void ReturnToIdle(void) {};
	virtual void CreateAttack(void) {};

    ANIM_TYPE bufferedAction_ = ANIM_TYPE::IDLE;
    int bufferTimer_ = 0;
    int hp_ = 100;

	int nextLightAttackType_ = -1;
	int nextHeavyAttackType_ = -1;
	bool isLightAttack_ = true;
	float comboBufferTimer_ = 0.0f;

};

