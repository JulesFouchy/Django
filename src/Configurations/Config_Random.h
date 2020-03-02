#pragma once

#include "Configurations.h"

#include <glm/glm.hpp>
#include <vector>

class Config_Random : public Configuration {
public:
	Config_Random();
	~Config_Random() = default;

	void embody(ParticlesSystem& particlesSystem) override;
	void generate(unsigned int nbParticles) override;

private:
	std::vector<glm::vec2> m_randomPositions;
};