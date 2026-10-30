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
		int animIndex = -1;
		float speed = 0.0f;
		float baseSpeed = 0.0f;
		float speedRate = 1.0f;
		float totalTime = 0.0f;
		float step = 0.0f;
		float progressTime = 0.0f;
		float duration = 0.0f;
		std::vector<SpeedRange> speedRanges;
	};
	//コンストラクタ
	AnimationController(int modelId);
	//デストラクタ
	~AnimationController(void);
	/// <summary>
	/// 外部ファイルによるアニメーションの追加
	/// </summary>
	/// <param name="type">アニメーションタイプ</param>
	/// <param name="speed">アニメーションスピード</param>
	/// <param name="path">パス</param>
	void Add(int type, float speed, const std::string path);
	/// <summary>
	/// モデル内部のアニメーションの追加
	/// </summary>
	/// <param name="type">アニメーションタイプ</param>
	/// <param name="speed">アニメーションスピード</param>
	/// <param name="animIndex">アニメーションのインデックス</param>
	void AddInFbx(int type, float speed, int animIndex);

	/// <summary>
	/// 割合で速度変更区間を追加する
	/// </summary>
	/// <param name="type">アニメーションタイプ</param>
	/// <param name="startRate">スタートの割合</param>
	/// <param name="endRate">エンドの割合</param>
	/// <param name="rate">アニメーションのスピード</param>
	void AddSpeedRange(int type, float startRate, float endRate, float rate);
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	/// <param name="type">アニメーションタイプ</param>
	/// <param name="isloop">ループするかどうか</param>
	void Play(int type, bool isloop = true);

	/// <summary>
	/// 指定したアニメーションをブレンドする
	/// </summary>
	/// <param name="type">アニメーションタイプ</param>
	/// <param name="weight">ブレンド割合</param>
	/// <param name="isloop">ループするかどうか</param>
	void SetBlendAnim(int type, float weight, bool isloop = true);
	// アタッチされているブレンド用アニメーションを削除する
	void RemoveBlendAnim(int type);
	// すべてのブレンドアニメーションを一括クリアする
	void ClearBlendAnims();
	//更新
	void Update(float timeScale=1.0f);
	//解放
	void Release(void);
	//アニメーションのトータルタイムの取得
	float GetTotalTime(void) const;
	//現在の進行の取得
	float GetCurrentStep(void) const;
	//現在の進歩割合の取得
	float GetProgressRate(void) const; 
	//再生タイプの取得
	int GetPlayType(void) const;
	//再生終了判定
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