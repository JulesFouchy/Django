#pragma once

#include "Configuration.h"

#include <glm/glm.hpp>
#include <vector>

class Config_Random : public Configuration {
public:
	Config_Random();
	~Config_Random() = default;

	void applyTo(ParticlesSystem& particlesSystem) override;
	bool reroll(unsigned int nbParticles) override;

private:
	std::vector<glm::vec2> m_randomPositions;
};