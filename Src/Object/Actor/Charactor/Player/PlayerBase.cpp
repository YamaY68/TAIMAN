#include "PlayerBase.h"
#include"../../Camera/CameraManager.h"
#include"../../../../Manager/Event/EventManager.h"
#include"../../../../Manager/Event/Events.h"

PlayerBase::PlayerBase(CameraManager*cameraManager)
{
	cameraManager_ = cameraManager;
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

	if (comboBufferTimer_ > 0.0f)
	{
		comboBufferTimer_--;
		if (comboBufferTimer_ <= 0.0f)
		{
			ClearCombo();
		}
	}
	if (KEY::GetInstance().GetInfo(KEY::KEY_TYPE::LOCK_ON).down)
	{
		if (cameraManager_)
		{
			LockOnEvent ev;
			ev.playerPos_ = transform_.pos;
			EventManager::GetInstance().Publish(ev);
		}
	}
	else
	{
		if (cameraManager_)
		{
			auto* cam = cameraManager_->FindCameraByKind(CAMERA_KIND::LOCK_ON);
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
	if (cameraManager_ && followCamera_)
	{
		cameraManager_->RemoveCamera(followCamera_.get());
	}
	followCamera_.reset();
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

void PlayerBase::IntendInputMoveDir(void)
{
	auto& key = KEY::GetInstance();
	VECTOR inputDir = { 0.0f, 0.0f, 0.0f };
	if (key.GetInfo(KEY::KEY_TYPE::MOVE_FRONT).now) inputDir.z += 1.0f;
	if (key.GetInfo(KEY::KEY_TYPE::MOVE_BACK).now)  inputDir.z -= 1.0f;
	if (key.GetInfo(KEY::KEY_TYPE::MOVE_RIGHT).now) inputDir.x += 1.0f;
	if (key.GetInfo(KEY::KEY_TYPE::MOVE_LEFT).now)  inputDir.x -= 1.0f;
	if (VSquareSize(inputDir) > 0.0001f)
	{	
		float cameraYaw = GetCameraManager()->GetCameraYaw();
		MATRIX camRotY = MGetRotY(cameraYaw);
		intendInputMoveDir_ = VNorm(VTransform(inputDir, camRotY));
	}
}
