#include "CameraEventHandler.h"
#include"../../../Object/Actor/Camera/LockOnVirtualCamera.h"
#include"../../../Object/Actor/Camera/CameraManager.h"
#include"../EventManager.h"
#include"../Events.h"
#include"../../../Object/Actor/ActorBase.h"
#include"../../../Object/Manager/ActorManager/ActorManager.h"
#include"../../../Object/Actor/Camera/PlayerFollowVirtualCamera.h"
#include"../../../Object/Actor/Camera/CameraManager.h"
#include"../../../Object/Actor/Charactor/Player/PlayerBase.h"


void CameraEventHandler::Init(GameContext* context)
{
    auto& ev = EventManager::GetInstance();

    ev.Subscribe<LockonToggleEvent>(this, [this](const LockonToggleEvent& e) {
        if (!cameraManager_ || !actorManager_) return;

        auto* lockCam = dynamic_cast<LockOnVirtualCamera*>(
            cameraManager_->FindCameraByKind(CAMERA_KIND::LOCKON));
        auto* followCam = dynamic_cast<PlayerFollowVirtualCamera*>(
            cameraManager_->FindCameraByKind(CAMERA_KIND::PLAYER_FOLLOW));
        auto* actor = actorManager_->FindActorByKind(ENTITY_KIND::PLAYER);
        auto* player = dynamic_cast<PlayerBase*>(actor);
        if (!lockCam || !followCam) return;

        // 【ロックオン解除処理】
        if (lockCam->HasTarget())
        {
            // 現在のカメラの最終描画ステートを取得
            const CameraState& curState = cameraManager_->GetState();

            // ロックオン終了時の視点（位置と注視点）をフォローカメラに引き継ぐ
            followCam->SyncOrientationFrom(curState.position, curState.target);

            // フォローカメラのUpdateを1回呼んで状態を即時反映させておく
            followCam->Update();

            // ロックオンカメラのターゲットを解除して非アクティブ化
            lockCam->ClearTarget();

            if (player)
            {
                player->SetLockonFlag(false);
            }
            return;
        }

        // 【ロックオン開始処理】
        ActorBase* enemy = actorManager_->FindNextLockonEnemy(e.playerPos_, 1500.0f, 0xffffffff);
        if (enemy)
        {
            lockCam->SetTarget(&enemy->GetTransform(), enemy->GetEntityID());
            lockCam->SetActive(true);

            if (player)
            {
                player->SetLockonFlag(true);
            }
        }
        });
}

void CameraEventHandler::Release(void)
{
	EventManager::GetInstance().Unsubscribe(this);
}
