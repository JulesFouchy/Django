#include "Config_ClusterRandom.h"

#include "Particles/ParticlesSystem.h"

#include "Helper/Random.h"

#include "Debugging/Log.h"

#include <imgui/imgui.h>

Config_ClusterRandom::Config_ClusterRandom()
	: Configuration("Cluster Random"), m_computeShader("res/shaders/configClusterRandom.comp"), m_nbClusters(1), m_a(43758.5453), m_v(12.9898, 78.233), m_delta(0.01f)
{}

void Config_ClusterRandom::ImGuiParameters(ParticlesSystem& particlesSystem) {
	bool change =
		ImGui::InputInt("Nb of Clusters", &m_nbClusters) ||
		ImGui::DragFloat2("X / Y variation", m_xyOff) ||
		ImGui::DragFloat("a", &m_a, 0.01f) ||
		ImGui::DragFloat2("v", (float*)&m_v) ||
		ImGui::DragFloat("delta", &m_delta, 0.01f);
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
	m_computeShader.get().setUniform1i("u_NbClusters", m_nbClusters);
	m_computeShader.get().setUniform1f("u_Seed", m_seed);
	m_computeShader.get().setUniform1f("a", m_a);
	m_computeShader.get().setUniform2f("v", m_v);
	m_computeShader.get().setUniform2f("xyOff", m_xyOff);
	m_computeShader.get().setUniform1f("delta", m_delta);
	m_computeShader.compute(m_nbParticles);
}

void Config_ClusterRandom::onKeyPressed(SDL_Scancode scancode, ParticlesSystem& partSystem) {
	if (scancode == SDL_SCANCODE_KP_PLUS) {
		m_nbClusters++;
		applyTo(partSystem);
	}
	else if (scancode == SDL_SCANCODE_KP_MINUS) {
		m_nbClusters--;
		applyTo(partSystem);
	}
}