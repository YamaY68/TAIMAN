#pragma once
#include <string>
#include <map>
#include <vector>

class AnimationController
{
public:

	// ① 速度を変化させる区間データ（割合：0.0 ? 1.0）
	struct SpeedRange
	{
		float startRate;   // 開始割合（例: 0.2 = 全体の20%地点）
		float endRate;     // 終了割合（例: 0.5 = 全体の50%地点）
		float rate;        // その区間の速度倍率
	};

	struct Animation
	{
		int model = -1;
		int attachNo = -1;
		int animIndex = 0;
		float speed = 0.0f;
		float baseSpeed = 0.0f;
		float speedRate = 1.0f;
		float totalTime = 0.0f;
		float step = 0.0f;
		float progressTime = 0.0f;
		float duration = 0.0f;
		std::vector<SpeedRange> speedRanges;
	};

	AnimationController(int modelId);
	~AnimationController(void);

	void Add(int type, float speed, const std::string path);
	void AddInFbx(int type, float speed, int animIndex);

	// ② 外から「割合（0.0?1.0）」で速度変更区間を追加する関数
	void AddSpeedRange(int type, float startRate, float endRate, float rate);

	void Play(int type, bool isloop = true);

	// 指定したタイプのアニメーションをアタッチし、ブレンド率（weight: 0.0?1.0）を設定して再生を続ける
	void SetBlendAnim(int type, float weight, bool isloop = true);
	// アタッチされているブレンド用アニメーションを削除する
	void RemoveBlendAnim(int type);
	// すべてのブレンドアニメーションを一括クリアする
	void ClearBlendAnims();

	void Update(void);
	void Release(void);

	float GetTotalTime(void) const;
	float GetCurrentStep(void) const;
	float GetProgressRate(void) const; // 現在の進捗割合（0.0 ? 1.0）を取得
	
	
	int GetPlayType(void) const;
	bool IsEnd(void) const;

private:
	int modelId_;
	std::map<int, Animation> animations_;
	int playType_;
	Animation playAnim_;
	
	struct ActiveBlendAnim {
		int type;
		Animation anim;
		float weight;    // ブレンド率 (0.0 ～ 1.0)
		bool isLoop;
	};
	std::vector<ActiveBlendAnim> blendAnims_; // 現在ブレンド合成されているアニメーションのリスト
	
	bool loopFlg_;


	void Add(int type, float speed, Animation& animation);
	void UpdateAnimInternal(Animation& anim, float deltaTime, bool isLoop);
};