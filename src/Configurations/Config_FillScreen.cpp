#include "Config_FillScreen.h"

#include "Particles/ParticlesSystem.h"

#include "Debugging/Log.h"

Config_FillScreen::Config_FillScreen() {
}

void Config_FillScreen::applyTo(ParticlesSystem& partSystem) {
	for (int i = 0; i < partSystem.size(); ++i) {
		partSystem[i].x = (i % m_nbRows) / (float)m_nbRows * 2.0f - 1.0f;
		partSystem[i].y = (i / m_nbRows) / (float)m_nbColumns * 2.0f - 1.0f;
	}
}

bool Config_FillScreen::reroll(unsigned int nbParticles) {
	m_nbRows = 44;
	m_nbColumns = 44;
	return true;
}