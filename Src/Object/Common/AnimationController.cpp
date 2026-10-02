#include <DxLib.h>
#include<algorithm>
#include "AnimationController.h"
#include "../../Manager/Game/SceneManager.h"
AnimationController::AnimationController(int modelId)
{
	modelId_ = modelId;
	playType_ = -1;
	loopFlg_ = false;
}
AnimationController::~AnimationController(void)
{
}
void AnimationController::Add(int type, float speed, const std::string path)
{
	Animation animation;
	animation.model = MV1LoadModel(path.c_str());
	animation.animIndex = -1;
	Add(type, speed, animation);

}
void AnimationController::AddInFbx(int type, float speed, int animIndex)
{
	Animation animation;
	animation.model = -1;
	animation.animIndex = animIndex;
	Add(type, speed, animation);
}
void AnimationController::AddSpeedRange(int type, float startRate, float endRate, float rate)
{
	if (animations_.count(type) > 0) {
		SpeedRange range;
		range.startRate = startRate;
		range.endRate = endRate;
		range.rate = rate;
		animations_[type].speedRanges.push_back(range);
	}
}
void AnimationController::Play(int type, bool isloop)
{
	if (playType_ == type) return;

	// ブレンドリストをクリアして単体に戻す
	ClearBlendAnims();

	if (playType_ != -1)
	{
		playAnim_.speedRate = 1.0f;
		playAnim_.speed = playAnim_.baseSpeed;


		// モデルからアニメーションを外す
		MV1DetachAnim(modelId_, playAnim_.attachNo);
	}

	// アニメーション種別を変更
	playType_ = type;
	playAnim_ = animations_[type];
	playAnim_.speedRate = 1.0f;
	// 初期化
	playAnim_.step = 0.0f;
	loopFlg_ = isloop;

	// 再生速度を初期速度×倍率で確定（切り替わり先がもしリセットされていれば初期速度になる）
	playAnim_.speed = playAnim_.baseSpeed * playAnim_.speedRate;

	if (playAnim_.model == -1) {
		// モデルと同じファイルからアニメーションをアタッチする
		playAnim_.attachNo = MV1AttachAnim(modelId_, playAnim_.animIndex);
	}
	else {
		//別のモデルファイルからアニメーションをアタッチする
		int animIdx = 0;
		playAnim_.attachNo = MV1AttachAnim(modelId_, animIdx, playAnim_.model);
	}

	// アニメーション総時間の取得
	playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);
	playAnim_.duration = GetTotalTime();
}

void AnimationController::SetBlendAnim(int type, float weight, bool isloop)
{
	if (animations_.count(type) == 0) return;

	// すでに同じタイプがブレンド中か探す
	auto it = std::find_if(blendAnims_.begin(), blendAnims_.end(), [type](const ActiveBlendAnim& b) {
		return b.type == type;
		});

	if (it != blendAnims_.end()) {
		// すでに存在する場合はウェイトのみ更新
		it->weight = std::clamp(weight, 0.0f, 1.0f);
		MV1SetAttachAnimBlendRate(modelId_, it->anim.attachNo, it->weight);
	}
	else {
		// 新規にブレンド対象として追加
		ActiveBlendAnim newBlend;
		newBlend.type = type;
		newBlend.anim = animations_[type];
		newBlend.weight = std::clamp(weight, 0.0f, 1.0f);
		newBlend.isLoop = isloop;
		newBlend.anim.speedRate = 1.0f;
		newBlend.anim.step = 0.0f;
		newBlend.anim.speed = newBlend.anim.baseSpeed;

		if (newBlend.anim.model == -1) {
			newBlend.anim.attachNo = MV1AttachAnim(modelId_, newBlend.anim.animIndex);
		}
		else {
			int animIdx = 0;
			newBlend.anim.attachNo = MV1AttachAnim(modelId_, animIdx, newBlend.anim.model);
		}

		newBlend.anim.totalTime = MV1GetAttachAnimTotalTime(modelId_, newBlend.anim.attachNo);
		MV1SetAttachAnimBlendRate(modelId_, newBlend.anim.attachNo, newBlend.weight);

		blendAnims_.push_back(newBlend);
	}
}

void AnimationController::RemoveBlendAnim(int type)
{
	auto it = std::remove_if(blendAnims_.begin(), blendAnims_.end(), [this, type](ActiveBlendAnim& b) {
		if (b.type == type) {
			if (b.anim.attachNo != -1) {
				MV1DetachAnim(modelId_, b.anim.attachNo);
			}
			return true;
		}
		return false;
		});
	blendAnims_.erase(it, blendAnims_.end());
}

void AnimationController::ClearBlendAnims()
{
	for (auto& b : blendAnims_) {
		if (b.anim.attachNo != -1) {
			MV1DetachAnim(modelId_, b.anim.attachNo);
		}
	}
	blendAnims_.clear();
}

void AnimationController::UpdateAnimInternal(Animation& anim, float deltaTime, bool isLoop)
{
	if (anim.attachNo == -1) return;

	float currentProgress = 0.0f;
	if (anim.totalTime > 0.0f) {
		currentProgress = anim.step / anim.totalTime;
		if (currentProgress > 1.0f) currentProgress = 1.0f;
	}

	float currentRate = 1.0f;
	for (const auto& range : anim.speedRanges)
	{
		if (currentProgress >= range.startRate && currentProgress < range.endRate)
		{
			currentRate = range.rate;
			break;
		}
	}

	anim.speedRate = currentRate;
	anim.speed = anim.baseSpeed * anim.speedRate;
	anim.step += (deltaTime * anim.speed);

	if (isLoop) {
		if (anim.step > anim.totalTime) {
			anim.step = fmod(anim.step, anim.totalTime);
		}
	}
	else {
		if (anim.step > anim.totalTime) {
			anim.step = anim.totalTime;
			anim.speedRate = 1.0f;
		}
	}

	MV1SetAttachAnimTime(modelId_, anim.attachNo, anim.step);
}

void AnimationController::Update(void)
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	if (playAnim_.attachNo != -1 && playAnim_.totalTime > 0.0f)
	{
		// 1. 進捗割合（0.0 ～ 1.0）
		float currentProgress = std::clamp(playAnim_.step / playAnim_.totalTime, 0.0f, 1.0f);

		// 2. 速度レートの判定
		float currentRate = 1.0f;
		for (const auto& range : playAnim_.speedRanges)
		{
			if (currentProgress >= range.startRate && currentProgress < range.endRate)
			{
				currentRate = range.rate;
				break;
			}
		}

		playAnim_.speedRate = currentRate;
		playAnim_.speed = playAnim_.baseSpeed * playAnim_.speedRate;

		// 3. アニメーション内ステップを進める
		playAnim_.step += (deltaTime * playAnim_.speed);

		// 上限判定は duration ではなく totalTime が基準！
		if (loopFlg_) {
			if (playAnim_.step >= playAnim_.totalTime) {
				playAnim_.step = fmod(playAnim_.step, playAnim_.totalTime);
			}
		}
		else {
			if (playAnim_.step >= playAnim_.totalTime) {
				playAnim_.step = playAnim_.totalTime;
			}
		}

		// DXライブラリへ反映
		MV1SetAttachAnimTime(modelId_, playAnim_.attachNo, playAnim_.step);

		// ブレンドウェイトの合計計算（メインの減衰）
		float blendWeightSum = 0.0f;
		for (const auto& b : blendAnims_) {
			blendWeightSum += b.weight;
		}
		float mainWeight = std::clamp(1.0f - blendWeightSum, 0.0f, 1.0f);
		MV1SetAttachAnimBlendRate(modelId_, playAnim_.attachNo, mainWeight);
	}

	// ブレンドアニメーションの更新
	for (auto& b : blendAnims_) {
		UpdateAnimInternal(b.anim, deltaTime, b.isLoop);
		if (b.anim.attachNo != -1) {
			MV1SetAttachAnimBlendRate(modelId_, b.anim.attachNo, b.weight);
		}
	}
}
void AnimationController::Release(void)
{
	if (playType_ != -1) {
		MV1DetachAnim(modelId_, playAnim_.attachNo);
	}

	//外部ロードしたFBXのアニメーションモデルのメモリ解放
	for (auto& pair : animations_) {
		if (pair.second.model != -1) {
			MV1DeleteModel(pair.second.model);
		}
	}

	//可変長配列をクリアする
	animations_.clear();
}

float AnimationController::GetTotalTime(void) const
{
	if (playType_ == -1) return 0.0f;
	float baseTotalTime = playAnim_.totalTime;
	if (baseTotalTime <= 0.0f) return 0.0f;
	if (playAnim_.speedRanges.empty()) return baseTotalTime;

	float actualTotalTime = 0.0f;
	std::vector<SpeedRange> sortedRanges = playAnim_.speedRanges;
	std::sort(sortedRanges.begin(), sortedRanges.end(), [](const SpeedRange& a, const SpeedRange& b) {
		return a.startRate < b.startRate;
	});

	float lastEndRate = 0.0f;
	for (const auto& range : sortedRanges)
	{
		if (range.startRate > lastEndRate) {
			actualTotalTime += (range.startRate - lastEndRate) * baseTotalTime;
		}
		float rangeBaseLength = (range.endRate - range.startRate) * baseTotalTime;
		float safeRate = (range.rate > 0.0001f) ? range.rate : 0.0001f;
		actualTotalTime += rangeBaseLength / safeRate;
		lastEndRate = (lastEndRate > range.endRate) ? lastEndRate : range.endRate;
	}
	if (lastEndRate < 1.0f) {
		actualTotalTime += (1.0f - lastEndRate) * baseTotalTime;
	}
	return actualTotalTime;
}
// 現在の再生時間（step）を取得する
float AnimationController::GetCurrentStep(void) const
{
	if (playType_ == -1) return 0.0f;
	return playAnim_.step; // ← 0.0f固定ではなく現在のステップを返す
}

// 現在の進捗割合（0.0 ? 1.0）を取得する
float AnimationController::GetProgressRate(void) const
{
	if (playType_ == -1 || playAnim_.totalTime <= 0.0f) return 0.0f;
	return std::clamp(playAnim_.step / playAnim_.totalTime, 0.0f, 1.0f);
}

bool AnimationController::IsEnd(void) const
{
	if (loopFlg_ || playType_ == -1) return false;
	return playAnim_.step >= playAnim_.totalTime;
}
void AnimationController::Add(int type, float speed, Animation& animation)
{
	if (animations_.count(type) == 0) {
		// 初期速度を保持し、スピード倍率は1.0fで初期化する
		animation.baseSpeed = speed;
		animation.speedRate = 1.0f;
		animation.speed = speed;

		animations_.emplace(type, animation);
	}
}

int AnimationController::GetPlayType(void) const
{
	return playType_;
}