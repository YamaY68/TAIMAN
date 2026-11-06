#include "PlayerBase.h"
#include"../../Camera/CameraManager.h"
#include"../../../../Manager/Event/EventManager.h"
#include"../../../../Manager/Event/Events.h"

PlayerBase::PlayerBase(CameraManager* cameraManager)
	: cameraManager_(cameraManager)
{
}

PlayerBase::~PlayerBase(void)
{
}

void PlayerBase::SubInit(void)
{

}

void PlayerBase::SubUpdate(void)
{
	IntendInputMoveDir();

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

	// コンボタイマーの減算
	if (comboBufferTimer_ > 0.0f)
	{
		comboBufferTimer_ -= 1.0f;
		if (comboBufferTimer_ <= 0.0f)
		{
			ClearCombo();
		}
	}
	// カメラマネージャーが存在しない場合は早期終了
	if (!cameraManager_) return;

	if (KEY::GetInstance().GetInfo(KEY::KEY_TYPE::LOCKON).down)
	{
		LockonToggleEvent ev;
		ev.playerPos_ = transform_.pos;
		EventManager::GetInstance().Publish(ev);
	}
}

void PlayerBase::SubDraw(void)
{
	DrawFormatString(0, 40, 0xffffff, "ComboBuffer: %.0f", comboBufferTimer_);
}

void PlayerBase::SubRelease(void)
{
	if (cameraManager_)
	{
		if (followCamera_)
		{
			cameraManager_->RemoveCamera(followCamera_.get());
		}
		if (lockOnCamera_)
		{
			cameraManager_->RemoveCamera(lockOnCamera_.get());
		}
	}
	followCamera_.reset();
	lockOnCamera_.reset();
}

void PlayerBase::SetNextComboChoices(ATTACK_TYPE lightType, ATTACK_TYPE heavyType)
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
	nextLightAttackType_ = ATTACK_TYPE::NONE;
	nextHeavyAttackType_ = ATTACK_TYPE::NONE;
	comboBufferTimer_ = 0.0f;
}

void PlayerBase::SetAttackType(bool isLight)
{
	isLightAttack_ = isLight;
}

VECTOR PlayerBase::CalculateCameraRelativeMoveDir(void) const
{
	auto& key = KEY::GetInstance();
	VECTOR inputDir = { 0.0f, 0.0f, 0.0f };
	if (key.GetInfo(KEY::KEY_TYPE::MOVE_FRONT).now) inputDir.z += 1.0f;
	if (key.GetInfo(KEY::KEY_TYPE::MOVE_BACK).now)  inputDir.z -= 1.0f;
	if (key.GetInfo(KEY::KEY_TYPE::MOVE_RIGHT).now) inputDir.x += 1.0f;
	if (key.GetInfo(KEY::KEY_TYPE::MOVE_LEFT).now)  inputDir.x -= 1.0f;

	if (VSquareSize(inputDir) <= 0.0001f)
	{
		return { 0.0f, 0.0f, 0.0f };
	}

	float cameraYaw = cameraManager_ ? cameraManager_->GetCameraYaw() : 0.0f;
	MATRIX camRotY = MGetRotY(cameraYaw);
	return VNorm(VTransform(inputDir, camRotY));
}

// PlayerBase.cpp
void PlayerBase::RotateTowardsIntendInput(void)
{
	// ★ 1. ロックオン中なら注視点（敵）の方向を最優先で向く
	if (isLockon_ && cameraManager_)
	{
		const auto& camState = cameraManager_->GetCurrentState();
		VECTOR toTarget = VSub(camState.target, transform_.pos);
		toTarget.y = 0.0f; // 水平方向のみ

		if (VSquareSize(toTarget) > 0.0001f)
		{
			float targetAngle = atan2f(toTarget.x, toTarget.z);
			transform_.quaRot = Quaternion::Euler(VGet(0.0f, targetAngle, 0.0f));
			return;
		}
	}

	// ★ 2. 非ロックオン時: スティック/キー入力があればその方向を向く
	if (VSquareSize(intendInputMoveDir_) > 0.0001f)
	{
		float angle = atan2f(intendInputMoveDir_.x, intendInputMoveDir_.z);
		transform_.quaRot = Quaternion::Euler(VGet(0.0f, angle, 0.0f));
	}
}

void PlayerBase::IntendInputMoveDir(void)
{
	VECTOR dir = CalculateCameraRelativeMoveDir();
	if (VSquareSize(dir) > 0.0001f)
	{
		intendInputMoveDir_ = dir;
	}
}


