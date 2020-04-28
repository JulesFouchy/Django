#pragma once

#include "Configuration.h"

#include "OpenGL/ComputeShader.h"

#include <glm/glm.hpp>
#include <vector>

enum class ClusterShape {
	Circle,
	Spiral,
	Star
};

class Config_ClusterRandom : public Configuration {
public:
	Config_ClusterRandom();
	~Config_ClusterRandom() = default;

	bool setup(unsigned int nbParticles) override;
	bool reroll() override;
	void applyTo(ParticlesSystem& particlesSystem) override;
	void onKeyPressed(SDL_Scancode scancode, ParticlesSystem& partSystem) override;

	void ImGuiParameters(ParticlesSystem& particlesSystem) override;

private:
	int m_nbClusters;
	ClusterShape m_clusterShape;

	float m_shapeRadius;
	float m_spiralRotationSpeed;

	float m_seed;
	float m_a;
	glm::vec2 m_v;
	float m_xyOff[2] = { 0.0f, 2.0f };
	float m_delta;

	ComputeShader m_computeShader;
};