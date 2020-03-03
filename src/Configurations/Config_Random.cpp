#include "Config_Random.h"

#include "Particles/ParticlesSystem.h"

#include "Helper/Random.h"

#include "Debugging/Log.h"

Config_Random::Config_Random()
	: m_computeShader("res/shaders/configRandom.comp")
{}

bool Config_Random::setup(unsigned int nbParticles) {
	if (Configuration::setup(nbParticles)) {
		//unsigned int prevSize = m_randomPositions.size();
		//m_randomPositions.resize(m_nbParticles);
		//if (prevSize < nbParticles) {
		//	for (int k = prevSize; k < nbParticles; ++k)
		//		m_randomPositions[k] = glm::vec2(MyRand::_m1to1(), MyRand::_m1to1());
		//}
		m_computeShader.get().bind();
		m_computeShader.get().setUniform1i("u_NbOfParticles", m_nbParticles);
		m_computeShader.get().unbind();
		reroll();
		return true;
	}
	return false;
}

bool Config_Random::reroll() {
	m_computeShader.get().bind();
	m_computeShader.get().setUniform1f("u_Seed", 100.0f * MyRand::_0to1());
	m_computeShader.compute(m_nbParticles);
	//for (int k = 0; k < m_nbParticles; ++k)
	//	m_randomPositions[k] = glm::vec2(MyRand::_m1to1(), MyRand::_m1to1());
	return true;
}

void Config_Random::applyTo(ParticlesSystem& partSystem) {
	reroll();
	//for (int i = 0; i < partSystem.size(); ++i) {
	//	partSystem[i] = m_randomPositions[i];
	//}
}