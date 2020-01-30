#include "PhysicsSettings.h"

#include "OpenGL/ShaderPipeline.h"

#include <imgui/imgui.h>

PhysicsSettings::PhysicsSettings(ShaderPipeline& physicsShader, float raideur, float dampening)
	: m_physicsShader(physicsShader), m_raideur(raideur), m_damping(dampening)
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
	m_physicsShader.setUniform1f("u_Damping", m_damping);
	m_physicsShader.unbind();
}

void PhysicsSettings::ImGui_Parameters() {
	if (ImGui::SliderFloat("Pull force", &m_raideur, 0.0f, 20.0f))
		setRaideurInShader();
	if (ImGui::SliderFloat("Damping", &m_damping, 0.0f, 20.0f))
		setDampeningInShader();
	if (ImGui::Button("Perfect")) {
		m_raideur = m_damping * m_damping / 4.0f;
		setRaideurInShader();
	}
}