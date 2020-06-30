#include "PhysicsSettings.h"

#include "Particles/ParticlesSystem.h"
#include "OpenGL/ShaderPipeline.h"

void PhysicsSettings::ImGui(ShaderPipeline& physicsCompute) {
	if (ImGui::SliderFloat("Pull force", &m_stiffness, 0.0f, 20.0f))
		setStiffnessInShader(physicsCompute);
	if (ImGui::SliderFloat("Damping", &m_damping, 0.0f, 20.0f))
		setDampingInShader(physicsCompute);
	if (ImGui::Button("Perfect")) {
		m_stiffness = m_damping * m_damping / 4.0f;
		setStiffnessInShader(physicsCompute);
	}
}

void PhysicsSettings::apply(ShaderPipeline& physicsCompute) {
	setStiffnessInShader(physicsCompute);
	setDampingInShader(physicsCompute);
}

void PhysicsSettings::setStiffnessInShader(ShaderPipeline& physicsCompute) {
	physicsCompute.bind();
	physicsCompute.setUniform1f("u_Stiffness", m_stiffness);
	physicsCompute.unbind();
}

void PhysicsSettings::setDampingInShader(ShaderPipeline& physicsCompute) {
	physicsCompute.bind();
	physicsCompute.setUniform1f("u_Damping", m_damping);
	physicsCompute.unbind();
}