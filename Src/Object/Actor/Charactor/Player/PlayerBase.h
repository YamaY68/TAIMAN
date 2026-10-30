#pragma once
#include "../../ActorBase.h"
#include"../../Camera/PlayerFollowVirtualCamera.h"
#include"../../Camera/LockOnVirtualCamera.h"
class CameraManager;
class PlayerBase :
    public ActorBase
{
public:
    enum class ANIMATION_TYPE
    {
		NONE = -1,
		IDLE,
		WALK,
		RUN,
		GUARD,
		DIVE,
		EVADE,
		PARRY,

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

	enum class ATTACK_TYPE
	{
		NONE=-1,

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

	//コンストラクタ
    PlayerBase(CameraManager*cameraManager);
	//デストラクタ
    virtual ~PlayerBase(void)override;
	//初期化
	void SubInit(void)override;
	//更新
    void SubUpdate(void)override;
	//描画
    void SubDraw(void)override;
	//解放
	void SubRelease(void)override;
	//攻撃リクエスト処理
	virtual void RequestAttack(void) = 0;
	/// <summary>
	/// 次回攻撃設定
	/// </summary>
	/// <param name="lightType">弱攻撃遷移先</param>
	/// <param name="heavyType">強攻撃遷移先</param>
	void SetNextComboChoices(ATTACK_TYPE lightType, ATTACK_TYPE heavyType);
	/// <summary>
	/// コンボ受付バッファ時間の設定
	/// </summary>
	/// <param name="frames"></param>
	void SetComboBuffer(float frames);
	//コンボ情報のクリア
	void ClearCombo(void);
	/// <summary>
	/// 攻撃種別の設定
	/// </summary>
	/// <param name="isLight"></param>
	void SetAttackType(bool isLight);
	//意図移動方向ベクトルの取得
	VECTOR GetIntendInputMoveDir(void) { return intendInputMoveDir_; }
	//カメラマネージャーの取得
	CameraManager* GetCameraManager(void) { return cameraManager_; }
	//カメラ向きを反映した入力方向ベクトルを取得
	VECTOR CalculateCameraRelativeMoveDir(void)const;
	//入力方向へアクターを即座に向ける
	void RotateTowardsIntendInput(void);
protected:
	virtual void ReturnToIdle(void) {};
	virtual void CreateAttack(void) {};
	void IntendInputMoveDir(void);
	VECTOR intendInputMoveDir_ = { 0.0f, 0.0f, 0.0f };
	ANIMATION_TYPE bufferedAction_ = ANIMATION_TYPE::IDLE;
	int bufferTimer_ = 0;
	int hp_ = 100;
	ATTACK_TYPE nextLightAttackType_ = ATTACK_TYPE::NONE;
	ATTACK_TYPE nextHeavyAttackType_ = ATTACK_TYPE::NONE;
	bool isLightAttack_ = true;
	float comboBufferTimer_ = 0.0f;
	CameraManager* cameraManager_ = nullptr;
	std::unique_ptr<PlayerFollowVirtualCamera> followCamera_ = nullptr;
	std::unique_ptr<LockOnVirtualCamera> lockOnCamera_ = nullptr;
};
