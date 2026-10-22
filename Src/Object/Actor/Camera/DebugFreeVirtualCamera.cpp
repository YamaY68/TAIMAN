#include "DebugFreeVirtualCamera.h"
#include <cmath>
#include <algorithm>

void DebugFreeVirtualCamera::Update(void)
{
	if (!isActive_) return;

	auto& key = KeyManager::GetInstance();

	// 1. 視点回転処理（キー入力による角度の更新）
	float rotSpeed = 0.03f;
	if (key.GetInfo(KEY::KEY_TYPE::CAMERA_ROTATE_RIGHT).now) yaw_ += rotSpeed;
	if (key.GetInfo(KEY::KEY_TYPE::CAMERA_ROTATE_LEFT).now)  yaw_ -= rotSpeed;
	if (key.GetInfo(KEY::KEY_TYPE::CAMERA_ROTATE_UP).now)    pitch_ -= rotSpeed;
	if (key.GetInfo(KEY::KEY_TYPE::CAMERA_ROTATE_DOWN).now)  pitch_ += rotSpeed;

	// 俯仰角 (Pitch) の制限（真上・真下を向いたときの画面反転防止）
	const float limitPitch = 89.0f * (DX_PI_F / 180.0f);
	pitch_ = std::clamp(pitch_, -limitPitch, limitPitch);

	// 2. yaw_ / pitch_ から視点方向（forward）および右・上ベクトルを算出
	float cosPitch = cosf(pitch_);
	float sinPitch = sinf(pitch_);
	float cosYaw = cosf(yaw_);
	float sinYaw = sinf(yaw_);

	VECTOR forward = VNorm(VGet(sinYaw * cosPitch, -sinPitch, cosYaw * cosPitch));
	VECTOR right = VNorm(VCross(VGet(0.0f, 1.0f, 0.0f), forward));
	VECTOR up = VGet(0.0f, 1.0f, 0.0f);

	// 3. 移動入力の合成（WASD + 上下移動）
	VECTOR moveDir = VGet(0.0f, 0.0f, 0.0f);

	// 前後移動（視点方向へ進む）
	if (key.GetInfo(KEY::KEY_TYPE::CAMERA_MOVE_FRONT).now) moveDir = VAdd(moveDir, forward);
	if (key.GetInfo(KEY::KEY_TYPE::CAMERA_MOVE_BACK).now)  moveDir = VSub(moveDir, forward);

	// 左右移動（視点に対する横方向へ進む）
	if (key.GetInfo(KEY::KEY_TYPE::CAMERA_MOVE_RIGHT).now) moveDir = VAdd(moveDir, right);
	if (key.GetInfo(KEY::KEY_TYPE::CAMERA_MOVE_LEFT).now)  moveDir = VSub(moveDir, right);

	// 上下移動（ワールドY軸に沿って昇降）
	if (key.GetInfo(KEY::KEY_TYPE::CAMERA_MOVE_UP).now)    moveDir = VAdd(moveDir, up);
	if (key.GetInfo(KEY::KEY_TYPE::CAMERA_MOVE_DOWN).now)  moveDir = VSub(moveDir, up);

	// 4. 位置と注視点の更新（衝突判定なしでダイレクトに移動）
	if (VSquareSize(moveDir) > 0.0001f)
	{
		// 斜め移動で速くならないよう正規化して指定スピードで動かす
		state_.position = VAdd(state_.position, VScale(VNorm(moveDir), speed_));
	}

	// 注視点は「位置 + 視点方向」
	state_.target = VAdd(state_.position, forward);
	state_.upVector = up;
}