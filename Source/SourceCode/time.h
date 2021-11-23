#pragma once
#include "../GameLib/singleton.h"

class TimeManager :public Singleton<TimeManager>
{
private:
	double frameTime = 1.0f;

public:
	float scale = 1.0f;

public:
	void SetFrame(double frame);
	void SetTimeScale(float scale) { this->scale = scale; }
	
	double DeltaTime() { return 1.0f / frameTime; }
	double ScaleDeltaTime() { return DeltaTime() * scale; }
};
#define GetTimeManager (TimeManager::GetInstance())