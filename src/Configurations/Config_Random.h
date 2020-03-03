#pragma once

#include "Configuration.h"

#include "OpenGL/ComputeShader.h"

#include <glm/glm.hpp>
#include <vector>

class Config_Random : public Configuration {
public:
	Config_Random();
	~Config_Random() = default;

	bool setup(unsigned int nbParticles) override;
	bool reroll() override;
	void applyTo(ParticlesSystem& particlesSystem) override;

	void ImGui();

private:
	//std::vector<glm::vec2> m_randomPositions;
	float m_seed;
	float m_a;
	glm::vec2 m_v;
	float m_off;
	float m_delta;
	ComputeShader m_computeShader;
};