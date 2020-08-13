#pragma once

class Time {
public:
	Time() = default;
	virtual ~Time() = default;

	virtual float deltaTime() const = 0;
	virtual float time() const = 0;

	virtual void update() = 0;
};