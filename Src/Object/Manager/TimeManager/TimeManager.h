#pragma once

class TimeManager
{
public:
	TimeManager(void);
	~TimeManager(void);
	void Update(void);

	void StartBulletTime(float duration, float scale = 0.5f)
	{
		bulletTimer_ = duration;
		bulletTimeScale_ = scale;
		isBulletTime_ = true;
	}

	float GetTimeScale(void);

private:

	float globalTimeScale_;
	float bulletTimer_;
	float bulletTimeScale_;
	bool isBulletTime_;
};

