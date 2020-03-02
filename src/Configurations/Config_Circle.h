#pragma once

#include "Configuration.h"

class Config_Circle : public Configuration {
public:
	Config_Circle();
	~Config_Circle() = default;

	void applyTo(ParticlesSystem& particlesSystem) override;
};