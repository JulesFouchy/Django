#pragma once

#include "Time.h"

class Time_Realtime : public Time {
public:
	Time_Realtime();
	static void Initialize();
	virtual ~Time_Realtime() = default;

	float deltaTime() override;
	float time() override;

	void update() override;

private:
	unsigned long long m_lastTime;
	unsigned long long m_currentTime;
	static float SDLPerformanceFrequency;
};