#include "Config_Circle.h"

#include "Particles/ParticlesSystem.h"

#include "Debugging/Log.h"

#include "Helper/DisplayInfos.h"

Config_Circle::Config_Circle()
	: Configuration("Circle")
{}

bool Config_Circle::setup(unsigned int nbParticles) {
	if (Configuration::setup(nbParticles)) {
		m_unitCircle.resize(nbParticles);
		for (int i = 0; i < nbParticles; ++i) {
			float angle = 2.0f * 3.1415927f * i / (float)nbParticles;
			m_unitCircle[i].x = cos(angle) / DisplayInfos::Ratio();
			m_unitCircle[i].y = sin(angle);
		}
		return true;
	}
	return false;
}

void Config_Circle::applyTo(ParticlesSystem& partSystem) {
	float radius = 0.8f;
	for (int i = 0; i < partSystem.size(); ++i)
		partSystem[i] = radius * m_unitCircle[i];
	partSystem.sendRestPositionsToGPU();
}