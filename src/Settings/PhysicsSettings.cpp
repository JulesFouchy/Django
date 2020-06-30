#include "PhysicsSettings.h"

#include "Particles/ParticlesSystem.h"
#include "OpenGL/ShaderPipeline.h"

PhysicsSettings::PhysicsSettings(float stiffness, float damping)
	: m_stiffness(stiffness), m_damping(damping)
{}

void PhysicsSettings::setUniforms(ShaderPipeline& shader) {
	setStiffnessInShader(shader);
	setDampingInShader(shader);
}

void PhysicsSettings::setStiffnessInShader(ShaderPipeline& shader) {
	shader.bind();
	shader.setUniform1f("u_Stiffness", m_stiffness);
	shader.unbind();
}

void PhysicsSettings::setDampingInShader(ShaderPipeline& shader) {
	shader.bind();
	shader.setUniform1f("u_Damping", m_damping);
	shader.unbind();
}

void PhysicsSettings::ImGui_Parameters(ShaderPipeline& shader) {
	if (ImGui::SliderFloat("Pull force", &m_stiffness, 0.0f, 20.0f))
		setStiffnessInShader(shader);
	if (ImGui::SliderFloat("Damping", &m_damping, 0.0f, 20.0f))
		setDampingInShader(shader);
	if (ImGui::Button("Perfect")) {
		m_stiffness = m_damping * m_damping / 4.0f;
		setStiffnessInShader(shader);
	}
}