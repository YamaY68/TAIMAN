#pragma once
#include "../../ActorBase.h"
#include"../../Camera/PlayerFollowVirtualCamera.h"
#include"../../Camera/LockOnVirtualCamera.h"
class CameraManager;
class PlayerBase :
    public ActorBase
{
public:
    enum class ACTION_TYPE
    {
		NONE = -1,
		IDLE,
		WALK,
		RUN,
		GUARD,
		DIVE,
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
		HEAVY_ATTACK_4,

		FINISH,
    };

    PlayerBase(CameraManager*cameraManager);
    virtual ~PlayerBase(void)override;

	void SubInit(void)override;
    void SubUpdate(void)override;
    void SubDraw(void)override;
	void SubRelease(void)override;

	virtual void RequestAttack(void) = 0;

	void SetNextComboChoices(int lightType, int heavyType);
	void SetComboBuffer(float frames);
	void ClearCombo(void);
	void SetAttackType(bool isLight);
	VECTOR GetIntendInputMoveDir(void) { return intendInputMoveDir_; }

	CameraManager* GetCameraManager(void) { return cameraManager_; }
protected:
	virtual void ReturnToIdle(void) {};
	virtual void CreateAttack(void) {};
	void IntendInputMoveDir(void);
	VECTOR intendInputMoveDir_;
    ACTION_TYPE bufferedAction_ = ACTION_TYPE::IDLE;
    int bufferTimer_ = 0;
    int hp_ = 100;
	int nextLightAttackType_ = -1;
	int nextHeavyAttackType_ = -1;
	bool isLightAttack_ = true;
	float comboBufferTimer_ = 0.0f;
	CameraManager* cameraManager_ = nullptr;
	std::unique_ptr<PlayerFollowVirtualCamera> followCamera_;
	std::unique_ptr<LockOnVirtualCamera>lockOnCamera_;
};
