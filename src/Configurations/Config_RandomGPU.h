#pragma once

#include "Configuration.h"

#include "OpenGL/ComputeShader.h"

class Config_RandomGPU : public Configuration {
public:
	Config_RandomGPU();
	~Config_RandomGPU() = default;

	bool setup(unsigned int nbParticles) override;
	bool reroll() override;
	void applyTo(ParticlesSystem& particlesSystem) override;

	void ImGuiParameters(ParticlesSystem& particlesSystem) override;

private:
	float m_seed;
	float m_a;
	glm::vec2 m_v;
	float m_xyOff[2] = { 0.0f, 2.0f };
	float m_delta;

	ComputeShader m_computeShader;
};