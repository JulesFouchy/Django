#pragma once

#include "Time.h"

class Time_Realtime : public Time {
public:
	Time_Realtime();
	static void Initialize();
	~Time_Realtime() = default;

	float deltaTime() override;
	float time() override;

	void update() override;

private:
	Uint64 m_lastTime;
	Uint64 m_currentTime;
	static float SDLPerformanceCounter2Seconds;
};