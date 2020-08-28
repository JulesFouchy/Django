#include "PhysicsSettings.h"

#include "OpenGL/ShaderPipeline.h"
#include "StateModifier.h"
#include "Recording/StateChange.h"
#include "Particles/ParticlesSystem.h"

PhysicsSettings::PhysicsSettings()
	: m_presets("djgPhysics")
{}

void PhysicsSettings::ImGui(StateModifier& stateModifier) {
	bool b = false;
	if (ImGui::SliderFloat("Pulsation", &m_values.pulsation, 0, 20)) {
		b = true;
		m_values.computePhysicalParameters();
		apply(stateModifier);
	}
	if (ImGui::SliderFloat("Damping Ratio", &m_values.dampingRatio, 0, 2)) {
		b = true;
		m_values.computeDamping();
		setDampingInShader(stateModifier);
	}
	if (ImGui::Button("Perfect")) {
		b = true;
		m_values.dampingRatio = 1;
		m_values.computeDamping();
		setDampingInShader(stateModifier);
	}
	if (m_presets.ImGui(&m_values)) {
		m_values.computePhysicalParameters();
		apply(stateModifier);
	}
	if (b)
		m_presets.setToPlaceholderSetting();
}

void PhysicsSettings::apply(StateModifier& stateModifier) {
	setStiffnessInShader(stateModifier);
	setDampingInShader(stateModifier);
}

void PhysicsSettings::setStiffnessInShader(StateModifier& stateModifier) {
	stateModifier.particleSystem().physicsComputeShader().bind();
	stateModifier.particleSystem().physicsComputeShader().setUniform1f("u_Stiffness", m_values.getStiffness());
	stateModifier.particleSystem().physicsComputeShader().unbind();
	stateModifier.recordChange({ StateChangeType::Physics_Stiffness, m_values.getStiffness() });
}

void PhysicsSettings::setDampingInShader(StateModifier& stateModifier) {
	stateModifier.particleSystem().physicsComputeShader().bind();
	stateModifier.particleSystem().physicsComputeShader().setUniform1f("u_Damping", m_values.getDamping());
	stateModifier.particleSystem().physicsComputeShader().unbind();
	stateModifier.recordChange({ StateChangeType::Physics_Damping, m_values.getDamping() });
}