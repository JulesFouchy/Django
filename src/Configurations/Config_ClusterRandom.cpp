#include "Config_ClusterRandom.h"

#include "Particles/ParticlesSystem.h"

#include "Helper/Random.h"
#include "Helper/DisplayInfos.h"

#include "Debugging/Log.h"

#include <imgui/imgui.h>

Config_ClusterRandom::Config_ClusterRandom()
	: Configuration("Cluster Random"), m_computeShader("res/shaders/configClusterRandom.comp"),
	m_nbClusters(1), m_clusterShape(ClusterShape::Circle),
	m_shapeRadius(0.2f), m_spiralRotationSpeed(15.0f),
	m_a(43758.5453), m_v(12.9898, 78.233), m_delta(0.01f)
{}

void Config_ClusterRandom::ImGuiParameters(ParticlesSystem& particlesSystem) {
	bool change = ImGui::InputInt("Nb of Clusters", &m_nbClusters);
	change |= ImGui::DragFloat2("X / Y variation", m_xyOff);
	change |= ImGui::DragFloat("a", &m_a, 0.01f);
	change |= ImGui::DragFloat2("v", (float*)&m_v);
	change |= ImGui::DragFloat("delta", &m_delta, 0.01f);
	ImGui::Separator();
	change |= ImGui::SliderFloat("Radius", &m_shapeRadius, 0.0f, 1.0f);
	switch (m_clusterShape)
	{
	case ClusterShape::Circle:
		break;
	case ClusterShape::Spiral:
		change |= ImGui::SliderFloat("Spiral Rotation", &m_spiralRotationSpeed, 0.0f, 20.0f);
		break;
	default:
		break;
	}
	if (change)
		applyTo(particlesSystem);
}

bool Config_ClusterRandom::setup(unsigned int nbParticles) {
	if (Configuration::setup(nbParticles)) {
		m_computeShader.get().bind();
		m_computeShader.get().setUniform1i("u_NbOfParticles", m_nbParticles);
		m_computeShader.get().unbind();
		reroll();
		return true;
	}
	return false;
}

bool Config_ClusterRandom::reroll() {
	m_seed = 100.0f * MyRand::_0to1();
	return true;
}

void Config_ClusterRandom::applyTo(ParticlesSystem& partSystem) {
	m_computeShader.get().bind();
	m_computeShader.get().setUniform1f("u_AspectRatio", DisplayInfos::Ratio());
	m_computeShader.get().setUniform1i("u_NbClusters", m_nbClusters);
	m_computeShader.get().setUniform1i("u_Shape", (int)m_clusterShape);
	m_computeShader.get().setUniform1f("u_Radius", m_shapeRadius);
	m_computeShader.get().setUniform1f("u_SpiralRotationSpeed", m_spiralRotationSpeed);
	m_computeShader.get().setUniform1f("u_Seed", m_seed);
	m_computeShader.get().setUniform1f("a", m_a);
	m_computeShader.get().setUniform2f("v", m_v);
	m_computeShader.get().setUniform2f("xyOff", m_xyOff);
	m_computeShader.get().setUniform1f("delta", m_delta);
	m_computeShader.compute(m_nbParticles);
}

void Config_ClusterRandom::onKeyPressed(SDL_Scancode scancode, ParticlesSystem& partSystem) {
	spdlog::info((int)scancode);
	if (scancode == SDL_SCANCODE_KP_PLUS) {
		m_nbClusters++;
		applyTo(partSystem);
	}
	else if (scancode == SDL_SCANCODE_KP_MINUS) {
		m_nbClusters--;
		applyTo(partSystem);
	}
	else if (scancode == SDL_SCANCODE_KP_0) {
		m_nbClusters = 0;
		applyTo(partSystem);
	}
	else if (scancode == SDL_SCANCODE_KP_1) {
		m_nbClusters = 1;
		applyTo(partSystem);
	}
	else if (scancode == SDL_SCANCODE_KP_2) {
		m_nbClusters = 10;
		applyTo(partSystem);
	}
	else if (scancode == SDL_SCANCODE_KP_3) {
		m_nbClusters = 100;
		applyTo(partSystem);
	}
	else if (scancode == 29) {
		m_clusterShape = ClusterShape::Circle;
		spdlog::info("dsf");
		applyTo(partSystem);
	}
	else if (scancode == SDL_SCANCODE_X) {
		spdlog::info("x");
		m_clusterShape = ClusterShape::Spiral;
		applyTo(partSystem);
	}
}