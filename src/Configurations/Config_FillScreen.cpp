#include "Config_FillScreen.h"

#include "Particles/ParticlesSystem.h"

Config_FillScreen::Config_FillScreen(ParticlesSystem& particlesSystem)
	: Configuration(particlesSystem)
{}

void Config_FillScreen::embody() {
}

void Config_FillScreen::generate() {
	m_nbRows = 10;
	m_nbColumns = 10;
}