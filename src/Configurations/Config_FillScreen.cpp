#include "Config_FillScreen.h"

#include "Particles/ParticlesSystem.h"

#include "Helper/DisplayInfos.h"

#include "Debugging/Log.h"

bool Config_FillScreen::setup(unsigned int nbParticles) {
	if (Configuration::setup(nbParticles)) {
		m_spacing = 2.0f * sqrt(DisplayInfos::Ratio() / nbParticles);;
		m_nbRows    = floor(2.0f / m_spacing);
		m_nbColumns = floor(2.0f * DisplayInfos::Ratio() / m_spacing);
		return true;
	}
	return false;
}

void Config_FillScreen::applyTo(ParticlesSystem& partSystem) {
	for (int i = 0; i < partSystem.size(); ++i) {
		int index = i < m_nbColumns * m_nbRows ? i : m_nbColumns * m_nbRows - 1;
		partSystem[i].x = ((index % m_nbColumns - (m_nbColumns - 1) * 0.5f) * m_spacing) / DisplayInfos::Ratio();
		partSystem[i].y =  (index / m_nbColumns - (m_nbRows    - 1) * 0.5f) * m_spacing;
	}
}