#pragma once

class Time {
public:
	Time() = default;
	virtual ~Time() = default;

	virtual float deltaTime() = 0;
	virtual float time() = 0;

	virtual void update() = 0;
};