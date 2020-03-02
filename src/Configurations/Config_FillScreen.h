#pragma once

#include "Configuration.h"

class Config_FillScreen : public Configuration {
public:
	Config_FillScreen();
	~Config_FillScreen() = default;

	void applyTo(ParticlesSystem& particlesSystem) override;
	bool reroll(unsigned int nbParticles) override;

private:
	int m_nbRows;
	int m_nbColumns;
};