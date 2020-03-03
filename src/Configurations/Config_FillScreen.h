#pragma once

#include "Configuration.h"

class Config_FillScreen : public Configuration {
public:
	Config_FillScreen();
	~Config_FillScreen() = default;

	bool setup(unsigned int nbParticles) override;
	void applyTo(ParticlesSystem& particlesSystem) override;

private:
	float m_spacing;
	int m_nbRows;
	int m_nbColumns;
};