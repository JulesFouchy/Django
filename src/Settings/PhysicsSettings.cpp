#include "PhysicsSettings.h"

#include "Particles/ParticlesSystem.h"
#include "OpenGL/ShaderPipeline.h"

PhysicsSettings::PhysicsSettings()
	: m_presets("djgPhysics")
{}

void PhysicsSettings::ImGui(ShaderPipeline& physicsCompute) {
	bool b = false;
	if (ImGui::SliderFloat("Pulsation", &m_values.pulsation, 0, 20)) {
		b = true;
		m_values.computePhysicalParameters();
		apply(physicsCompute);
	}
	if (ImGui::SliderFloat("Damping Ratio", &m_values.dampingRatio, 0, 2)) {
		b = true;
		m_values.computeDamping();
		setDampingInShader(physicsCompute);
	}
	if (ImGui::Button("Perfect")) {
		b = true;
		m_values.dampingRatio = 1;
		m_values.computeDamping();
		setDampingInShader(physicsCompute);
	}
	if (m_presets.ImGui(&m_values)) {
		m_values.computePhysicalParameters();
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
	physicsCompute.setUniform1f("u_Stiffness", m_values.getStiffness());
	physicsCompute.unbind();
}

void PhysicsSettings::setDampingInShader(ShaderPipeline& physicsCompute) {
	physicsCompute.bind();
	physicsCompute.setUniform1f("u_Damping", m_values.getDamping());
	physicsCompute.unbind();
}