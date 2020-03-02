#pragma once

#include "Configurations.h"

class Config_Circle : public Configuration {
public:
	Config_Circle();
	~Config_Circle() = default;

	void embody(ParticlesSystem& particlesSystem) override;
};