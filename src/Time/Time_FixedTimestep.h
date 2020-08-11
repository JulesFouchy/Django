#pragma once

#include "Time.h"

class Time_FixedTimestep : public Time {
public:
	Time_FixedTimestep(float fps);
	~Time_FixedTimestep() = default;

	float deltaTime() override;
	float time() override;

	void update() override;

private:
	float m_dt;
	unsigned int m_framesCount = 0; // store time as an int to avoid floating point imprecisions when adding a small number (dt) to a big one (time) [don't know if this is really necessary though]
};