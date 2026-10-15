#pragma once

class TimeManager
{
public:
	static void CreateInstance(void);
	static TimeManager& GetInstance(void);
	static void DeleteInstance(void);

	void Update(void);

	void StartBulletTime(float duration, float scale = 0.5f)
	{
		bulletTimer_ = duration;
		bulletTimeScale_ = scale;
		isBulletTime_ = true;
	}

	float GetTimeScale(void);

private:
	static TimeManager* instance_;
	TimeManager(void);
	TimeManager(const TimeManager& instance) = default;
	~TimeManager(void) = default;

	float globalTimeScale_;
	float bulletTimer_;
	float bulletTimeScale_;
	bool isBulletTime_;
};

