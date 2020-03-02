#include "Config_Random.h"

#include "Particles/ParticlesSystem.h"

#include "Helper/Random.h"

#include "Debugging/Log.h"

Config_Random::Config_Random() {
}

void Config_Random::embody(ParticlesSystem& partSystem) {
	for (int i = 0; i < partSystem.size(); ++i) {
		partSystem[i] = m_randomPositions[i];
	}
}

void Config_Random::generate(unsigned int nbParticles) {
	m_randomPositions.resize(nbParticles);
	for (int k = 0; k < nbParticles; ++k)
		m_randomPositions[k] = glm::vec2(MyRand::_m1to1(), MyRand::_m1to1());
}