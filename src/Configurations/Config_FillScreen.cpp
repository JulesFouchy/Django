#include "Config_FillScreen.h"

#include "Particles/ParticlesSystem.h"

#include "Debugging/Log.h"

Config_FillScreen::Config_FillScreen(ParticlesSystem& particlesSystem)
	: Configuration(particlesSystem)
{
	generate();
}

void Config_FillScreen::embody() {
	for (int i = 0; i < m_partSystem.size(); ++i) {
		m_partSystem[i].x = (i % m_nbRows) / (float)m_nbRows * 2.0f - 1.0f;
		m_partSystem[i].y = (i / m_nbRows) / (float)m_nbColumns * 2.0f - 1.0f;
	}
}

void Config_FillScreen::generate() {
	m_nbRows = 44;
	m_nbColumns = 44;
}