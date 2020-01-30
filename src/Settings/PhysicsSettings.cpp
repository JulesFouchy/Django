#include "PhysicsSettings.h"

#include "OpenGL/ShaderPipeline.h"

#include <imgui/imgui.h>

PhysicsSettings::PhysicsSettings(ShaderPipeline& physicsShader, float stiffness, float damping)
	: m_physicsShader(physicsShader), m_stiffness(stiffness), m_damping(damping)
{}

void PhysicsSettings::setUniforms() {
	setStiffnessInShader();
	setDampingInShader();
}

void PhysicsSettings::setStiffnessInShader() {
	m_physicsShader.bind();
	m_physicsShader.setUniform1f("u_Stiffness", m_stiffness);
	m_physicsShader.unbind();
}

void PhysicsSettings::setDampingInShader() {
	m_physicsShader.bind();
	m_physicsShader.setUniform1f("u_Damping", m_damping);
	m_physicsShader.unbind();
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