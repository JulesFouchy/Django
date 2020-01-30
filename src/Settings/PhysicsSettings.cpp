#include "PhysicsSettings.h"

#include "OpenGL/ShaderPipeline.h"

#include <imgui/imgui.h>

PhysicsSettings::PhysicsSettings(ShaderPipeline& physicsShader, float raideur, float dampening)
	: m_physicsShader(physicsShader), m_raideur(raideur), m_dampening(dampening)
{}

void PhysicsSettings::setUniforms() {
	setRaideurInShader();
	setDampeningInShader();
}

void PhysicsSettings::setRaideurInShader() {
	m_physicsShader.bind();
	m_physicsShader.setUniform1f("u_Raideur", m_raideur);
	m_physicsShader.unbind();
}

void PhysicsSettings::setDampeningInShader() {
	m_physicsShader.bind();
	m_physicsShader.setUniform1f("u_Dampening", m_dampening);
	m_physicsShader.unbind();
}

void PhysicsSettings::ImGui_Parameters() {
	if (ImGui::SliderFloat("Pull force", &m_raideur, 0.0f, 20.0f))
		setRaideurInShader();
	if (ImGui::SliderFloat("Dampening", &m_dampening, 0.0f, 20.0f))
		setDampeningInShader();
}