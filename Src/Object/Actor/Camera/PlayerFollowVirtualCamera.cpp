#include "PlayerFollowVirtualCamera.h"
#include "../../../Utility/AsoUtility.h"
#include <algorithm>

void PlayerFollowVirtualCamera::Update(void)
{
    if (!playerTrans_) return;

    // プレイヤーの理想の注視点（胸の高さ）
    VECTOR playerChestPos = playerTrans_->pos;
    playerChestPos.y += height_;

    // 初回フレームの初期化
    if (isFirstFrame_)
    {
        currentLookAt_ = playerChestPos;

        Quaternion rotY = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);
        Quaternion rotX = Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X);
        Quaternion initRot = rotY.Mult(rotX);

        VECTOR localOffset = { 0.0f, 0.0f, -distance_ };
        currentCamPos_ = VAdd(currentLookAt_, initRot.PosAxis(localOffset));

        state_.position = currentCamPos_;
        state_.target = currentLookAt_;
        isFirstFrame_ = false;
    }

    // 1. スティック / キー入力による旋回操作
    ProcessRotation();

    // 入力による回転を反映したカメラ位置を更新
    Quaternion rotY = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);
    Quaternion rotX = Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X);
    Quaternion currentRot = rotY.Mult(rotX);

    VECTOR localOffset = { 0.0f, 0.0f, -distance_ };
    currentCamPos_ = VAdd(currentLookAt_, currentRot.PosAxis(localOffset));

    // 2. デッドゾーン判定
    VECTOR deadZoneOffset = CalculateDeadZoneOffset(playerChestPos);

    if (VSize(deadZoneOffset) > 0.001f)
    {
        // 【枠外に出た場合】はみ出た分だけ即座／強めに追従
        currentLookAt_ = VAdd(currentLookAt_, deadZoneOffset);
        currentCamPos_ = VAdd(currentCamPos_, deadZoneOffset);
    }
    else
    {
        // ★【枠内にいる場合】イージング（Lerp）でプレイヤーをゆっくり中央へ復帰させる
        // 0.02f ? 0.05f 程度にすると、遊びを感じつつ自然に中央へ戻ります
        const float returnToCenterSpeed = 0.05f;
        VECTOR toCenter = VSub(playerChestPos, currentLookAt_);

        // 水平方向（XZ）を中心に引き戻す
        VECTOR horizontalShift = VGet(toCenter.x * returnToCenterSpeed, 0.0f, toCenter.z * returnToCenterSpeed);
        currentLookAt_ = VAdd(currentLookAt_, horizontalShift);
        currentCamPos_ = VAdd(currentCamPos_, horizontalShift);
    }

    // Y軸（ジャンプや高低差）の追従
    float diffY = (playerChestPos.y - currentLookAt_.y) * 0.1f;
    currentLookAt_.y += diffY;
    currentCamPos_.y += diffY;

    // 3. 壁めり込み回避
    VECTOR finalCamPos = ResolveWallCollision(currentLookAt_, currentCamPos_);

    // 4. カメラ状態を更新
    state_.position = finalCamPos;
    state_.target = currentLookAt_;
    state_.upVector = VGet(0.0f, 1.0f, 0.0f);

    // 水平を保った姿勢を確定
    VECTOR dirToTarget = VSub(state_.target, state_.position);
    state_.rotation = Quaternion::LookRotation(dirToTarget);
}
void PlayerFollowVirtualCamera::ProcessRotation(void)
{
	auto& key = KeyManager::GetInstance();
	float rotSpeed = 0.03f;

	if (key.GetInfo(KEY::KEY_TYPE::CAMERA_ROTATE_RIGHT).now) angles_.y += rotSpeed;
	if (key.GetInfo(KEY::KEY_TYPE::CAMERA_ROTATE_LEFT).now)  angles_.y -= rotSpeed;
	if (key.GetInfo(KEY::KEY_TYPE::CAMERA_ROTATE_UP).now)    angles_.x -= rotSpeed;
	if (key.GetInfo(KEY::KEY_TYPE::CAMERA_ROTATE_DOWN).now)  angles_.x += rotSpeed;

	const float limitUp = 50.0f * (DX_PI_F / 180.0f);
	const float limitDown = -10.0f * (DX_PI_F / 180.0f);
	angles_.x = std::clamp(angles_.x, limitDown, limitUp);
}

VECTOR PlayerFollowVirtualCamera::ResolveWallCollision(const VECTOR& target, const VECTOR& idealPos)
{
	if (stageModelId_ == -1) return idealPos;

	// 注視点から理想位置へのレイキャスト
	MV1_COLL_RESULT_POLY_DIM hits = MV1CollCheck_LineDim(stageModelId_, -1, target, idealPos);
	VECTOR resultPos = idealPos;

	if (hits.HitNum > 0)
	{
		float minDist = 99999.0f;

		for (int i = 0; i < hits.HitNum; ++i)
		{
			float dist = VSize(VSub(hits.Dim[i].HitPosition, target));
			if (dist < minDist)
			{
				minDist = dist;
				VECTOR dir = VNorm(VSub(idealPos, target));
				// 壁ポリゴンの表面から少し手前（10.0f）にカメラを置く
				resultPos = VSub(hits.Dim[i].HitPosition, VScale(dir, 10.0f));
			}
		}
	}
	MV1CollResultPolyDimTerminate(hits);
	return resultPos;
}