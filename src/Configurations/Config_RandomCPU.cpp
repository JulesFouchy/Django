#include "Config_RandomCPU.h"

#include "Particles/ParticlesSystem.h"

#include "Helper/Random.h"

#include "Debugging/Log.h"

Config_RandomCPU::Config_RandomCPU()
	: Configuration("Random CPU")
{}

bool Config_RandomCPU::setup(unsigned int nbParticles) {
	if (Configuration::setup(nbParticles)) {
		unsigned int prevSize = m_randomPositions.size();
		m_randomPositions.resize(m_nbParticles);
		if (prevSize < nbParticles) {
			for (int k = prevSize; k < nbParticles; ++k)
				m_randomPositions[k] = glm::vec2(MyRand::_m1to1(), MyRand::_m1to1());
		}
		return true;
	}
	return false;
}

bool Config_RandomCPU::reroll() {
	for (int k = 0; k < m_nbParticles; ++k)
		m_randomPositions[k] = glm::vec2(MyRand::_m1to1(), MyRand::_m1to1());
	return true;
}

void Config_RandomCPU::applyTo(ParticlesSystem& partSystem) {
	for (int i = 0; i < partSystem.size(); ++i) {
		partSystem[i] = m_randomPositions[i];
	}
	partSystem.sendRestPositionsToGPU();
}