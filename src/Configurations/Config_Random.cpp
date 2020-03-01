#include "Config_Random.h"

#include "Particles/ParticlesSystem.h"

#include "Helper/Random.h"
#include "Settings/VisualSettings.h"

#include "Debugging/Log.h"

Config_Random::Config_Random() {
	generate();
}

void Config_Random::embody(ParticlesSystem& partSystem) {
	for (int i = 0; i < partSystem.size(); ++i) {
		partSystem[i] = m_randomPositions[i];
	}
}

void Config_Random::generate() {
	int N = VisualSettings::NbOfParticles();
	m_randomPositions.resize(N);
	for (int k = 0; k < N; ++k)
		m_randomPositions[k] = glm::vec2(MyRand::_m1to1(), MyRand::_m1to1());
}