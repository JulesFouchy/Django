#pragma once

#include "Configurations.h"

#include <glm/glm.hpp>
#include <vector>

class Config_Random : public Configuration {
public:
	Config_Random(ParticlesSystem& particlesSystem);
	~Config_Random() = default;

	void embody() override;
	void generate() override;

private:
	std::vector<glm::vec2> m_randomPositions;
};