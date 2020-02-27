#include "PhysicsSettings.h"

#include "Particles/ParticlesSystem.h"

#include <imgui/imgui.h>

PhysicsSettings::PhysicsSettings(float stiffness, float damping)
	: m_stiffness(stiffness), m_damping(damping)
{}

void PhysicsSettings::setUniforms() {
	setStiffnessInShader();
	setDampingInShader();
}

void PhysicsSettings::setStiffnessInShader() {
	ParticlesSystem::PhysicsComputeShader().bind();
	ParticlesSystem::PhysicsComputeShader().setUniform1f("u_Stiffness", m_stiffness);
	ParticlesSystem::PhysicsComputeShader().unbind();
}

void PhysicsSettings::setDampingInShader() {
	ParticlesSystem::PhysicsComputeShader().bind();
	ParticlesSystem::PhysicsComputeShader().setUniform1f("u_Damping", m_damping);
	ParticlesSystem::PhysicsComputeShader().unbind();
}

void PhysicsSettings::ImGui_Parameters() {
	if (ImGui::SliderFloat("Pull force", &m_stiffness, 0.0f, 20.0f))
		setStiffnessInShader();
	if (ImGui::SliderFloat("Damping", &m_damping, 0.0f, 20.0f))
		setDampingInShader();
	if (ImGui::Button("Perfect")) {
		m_stiffness = m_damping * m_damping / 4.0f;
		setStiffnessInShader();
	}
}