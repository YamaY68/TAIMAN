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

	if (KEY::GetInstance().GetInfo(KEY::KEY_TYPE::LOCK_ON).down)
	{
		LockOnEvent ev;
		ev.playerPos_ = transform_.pos;
		EventManager::GetInstance().Publish(ev);
	}
	else
	{
		auto* cam = cameraManager_->FindCameraByKind(CAMERA_KIND::LOCK_ON);
		if (cam)
		{
			cam->SetActive(false);
		}
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

void PlayerBase::RotateTowardsIntendInput(void)
{
	if (VSquareSize(intendInputMoveDir_) > 0.0001f)
	{
		transform_.quaRot = Quaternion::Euler(VGet(0.0f, atan2f(intendInputMoveDir_.x, intendInputMoveDir_.z), 0.0f));
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
