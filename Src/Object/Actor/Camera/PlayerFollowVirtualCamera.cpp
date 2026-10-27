#include "PlayerFollowVirtualCamera.h"
#include "../../../Utility/AsoUtility.h"
#include <algorithm>

void PlayerFollowVirtualCamera::Update(void)
{
	if (!playerTrans_) return;

	// 1. スティック/キー入力による旋回
	ProcessRotation();

	// 2. 注視点（プレイヤーの胸付近）
	VECTOR targetPos = playerTrans_->pos;
	targetPos.y += height_;

	// 3. 回転クォータニオンの合成
	Quaternion rotY = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);
	Quaternion rotX = Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X);
	state_.rotation = rotY.Mult(rotX);

	// 4. 回転後の理想的なカメラ位置を算出
	VECTOR localOffset = { 0.0f, 0.0f, -distance_ };
	VECTOR worldOffset = state_.rotation.PosAxis(localOffset);
	VECTOR idealCamPos = VAdd(targetPos, worldOffset);

	// 5. 壁めり込み回避（二重宣言を修正）
	VECTOR finalCamPos = ResolveWallCollision(targetPos, idealCamPos);

	state_.position = finalCamPos;
	state_.target = targetPos;
}

void PlayerFollowVirtualCamera::ProcessRotation(void)
{
	auto& key = KeyManager::GetInstance();
	float rotSpeed = 0.03f;

		if (key.GetInfo(KEY::KEY_TYPE::CAMERA_ROTATE_RIGHT).now)
		{
			angles_.y += rotSpeed;
		}

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