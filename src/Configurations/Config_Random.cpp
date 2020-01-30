#include "Config_Random.h"

#include "Particles/ParticlesSystem.h"

#include "Helper/Random.h"

#include "Debugging/Log.h"

Config_Random::Config_Random(ParticlesSystem& particlesSystem)
	: Configuration(particlesSystem)
{
	generate();
}

void Config_Random::embody() {
	for (int i = 0; i < m_partSystem.size(); ++i) {
		m_partSystem[i] = m_randomPositions[i];
	}
}

void Config_Random::generate() {
	m_randomPositions.resize(m_partSystem.size());
	for (int k = 0; k < m_partSystem.size(); ++k)
		m_randomPositions[k] = glm::vec2(MyRand::_m1to1(), MyRand::_m1to1());
}