#include "Config_RandomGPU.h"

#include "Particles/ParticlesSystem.h"

#include "Helper/Random.h"

#include "Debugging/Log.h"

#include <imgui/imgui.h>

Config_RandomGPU::Config_RandomGPU()
	: Configuration("Random GPU"), m_computeShader("res/shaders/configRandom.comp"), m_a(43758.5453), m_v(12.9898, 78.233), m_delta(0.01f)
{}

void Config_RandomGPU::ImGuiParameters(ParticlesSystem& particlesSystem) {
	bool change = ImGui::DragFloat2("X / Y variation", m_xyOff);
	change |= ImGui::DragFloat("a", &m_a, 0.01f);
	change |= ImGui::DragFloat2("v", (float*)&m_v);
	change |= ImGui::DragFloat("delta", &m_delta, 0.01f);
	if (change)
		applyTo(particlesSystem);
}

bool Config_RandomGPU::setup(unsigned int nbParticles) {
	if (Configuration::setup(nbParticles)) {
		m_computeShader.get().bind();
		m_computeShader.get().setUniform1i("u_NbOfParticles", m_nbParticles);
		m_computeShader.get().unbind();
		reroll();
		return true;
	}
	return false;
}

bool Config_RandomGPU::reroll() {
	m_seed = 100.0f * MyRand::_0to1();
	return true;
}

void Config_RandomGPU::applyTo(ParticlesSystem& partSystem) {
	m_computeShader.get().bind();
	m_computeShader.get().setUniform1f("u_Seed", m_seed);
	m_computeShader.get().setUniform1f("a", m_a);
	m_computeShader.get().setUniform2f("v", m_v);
	m_computeShader.get().setUniform2f("xyOff", m_xyOff);
	m_computeShader.get().setUniform1f("delta", m_delta);
	m_computeShader.compute(m_nbParticles);
}