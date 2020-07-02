#include "PhysicsSettings.h"

#include "Particles/ParticlesSystem.h"
#include "OpenGL/ShaderPipeline.h"

PhysicsSettings::PhysicsSettings()
	: m_presets("djgPhysics", "C:/Dev/Django/settings")
{}

void PhysicsSettings::ImGui(ShaderPipeline& physicsCompute) {
    if (ImGui::SliderFloat("Pull force", &m_values.stiffness, 0.0f, 20.0f)) {
        setStiffnessInShader(physicsCompute);
		m_presets.setToPlaceholderSetting();
    }
    if (ImGui::SliderFloat("Damping", &m_values.damping, 0.0f, 20.0f)) {
        setDampingInShader(physicsCompute);
		m_presets.setToPlaceholderSetting();
    }
	if (ImGui::Button("Perfect")) {
		m_values.stiffness = m_values.damping * m_values.damping / 4.0f;
		setStiffnessInShader(physicsCompute);
		m_presets.setToPlaceholderSetting();
	}
	if (m_presets.ImGui(&m_values)) {
		apply(physicsCompute);
	}
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