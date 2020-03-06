#include "Configuration.h"

Configuration::Configuration(const std::string& name)
	: m_nbParticles(-1), m_name(name)
{}

bool Configuration::setup(unsigned int nbParticles) {
	// returns wheter nbParticles changed
	bool change = (m_nbParticles != nbParticles);
	m_nbParticles = nbParticles;
	return change;
};

void Configuration::onKeyPressed(SDL_Scancode scancode, ParticlesSystem& partSystem) {}