#pragma once

#include "Configuration.h"

class Config_Circle : public Configuration {
public:
	Config_Circle();
	~Config_Circle() = default;

	bool setup(unsigned int nbParticles) override;
	void applyTo(ParticlesSystem& particlesSystem) override;

	void ImGuiParameters(ParticlesSystem& particlesSystem) override;

private:
	std::vector<glm::vec2> m_unitCircle;

	float m_radius;
	float m_rotation;
};