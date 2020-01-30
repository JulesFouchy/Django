#pragma once

#include "Time.h"

class Time_Realtime : public Time {
public:
	Time_Realtime() = default;
	static void Initialize();
	virtual ~Time_Realtime() = default;

	float deltaTime() override;
	float time() override;

	void update() override;

private:
	unsigned long long m_lastTime = 0;
	unsigned long long m_currentTime = 0;
	static float SDLPerformanceFrequency;
};