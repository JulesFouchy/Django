#include "PhysicsSettings.h"

#include "Particles/ParticlesSystem.h"
#include "OpenGL/ShaderPipeline.h"

PhysicsSettings::PhysicsSettings()
	: m_presets("djgPhysics")
{}

void PhysicsSettings::ImGui(ShaderPipeline& physicsCompute) {
	bool b = false;
    if (ImGui::SliderFloat("Pull force", &m_values.stiffness, 0.0f, 20.0f)) {
		b = true;
        setStiffnessInShader(physicsCompute);
    }
    if (ImGui::SliderFloat("Damping", &m_values.damping, 0.0f, 20.0f)) {
		b = true;
        setDampingInShader(physicsCompute);
    }
	if (ImGui::Button("Perfect")) {
		b = true;
		m_values.stiffness = m_values.damping * m_values.damping / 4.0f;
		setStiffnessInShader(physicsCompute);
	}
	if (m_presets.ImGui(&m_values)) {
		apply(physicsCompute);
	}
	if (b)
		m_presets.setToPlaceholderSetting();
}

void PhysicsSettings::apply(ShaderPipeline& physicsCompute) {
	setStiffnessInShader(physicsCompute);
	setDampingInShader(physicsCompute);
}

void PhysicsSettings::setStiffnessInShader(ShaderPipeline& physicsCompute) {
	physicsCompute.bind();
	physicsCompute.setUniform1f("u_Stiffness", m_values.stiffness);
	physicsCompute.unbind();
}

void PhysicsSettings::setDampingInShader(ShaderPipeline& physicsCompute) {
	physicsCompute.bind();
	physicsCompute.setUniform1f("u_Damping", m_values.damping);
	physicsCompute.unbind();
}