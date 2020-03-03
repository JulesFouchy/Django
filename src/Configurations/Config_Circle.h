#pragma once

#include "Configuration.h"

#include <glm/glm.hpp>
#include <vector>

class Config_Circle : public Configuration {
public:
	Config_Circle() = default;
	~Config_Circle() = default;

	bool setup(unsigned int nbParticles) override;
	void applyTo(ParticlesSystem& particlesSystem) override;

private:
	std::vector<glm::vec2> m_unitCircle;
};