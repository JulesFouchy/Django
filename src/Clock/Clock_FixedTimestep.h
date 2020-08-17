#pragma once

#include "Clock.h"

class Clock_FixedTimestep : public Clock {
public:
	Clock_FixedTimestep(float fps);
	~Clock_FixedTimestep() = default;

	float deltaTime() const override;
	float time() const override;

	void setTime(float newTime) override;
	void update() override;

private:
	const float m_dt;
	unsigned int m_framesCount = 0; // store time as an int to avoid floating point imprecisions when adding a small number (dt) to a big one (time) [don't know if this is really necessary though]
};