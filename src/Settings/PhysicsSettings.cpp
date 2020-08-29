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
		applyAndRecord_Pulsation(stateModifier);
	}
	if (ImGui::SliderFloat("Damping Ratio", &m_values.dampingRatio, 0, 2)) {
		b = true;
		applyAndRecord_DampingRatio(stateModifier);
	}
	if (ImGui::Button("Perfect")) {
		b = true;
		setDampingRatio(1.0f);
		applyAndRecord_DampingRatio(stateModifier);
	}
	if (m_presets.ImGui(&m_values)) {
		applyAndRecord(stateModifier);
	}
	if (b)
		m_presets.setToPlaceholderSetting();
}

void PhysicsSettings::applyAndRecord(StateModifier& stateModifier) {
	m_values.computePhysicalParameters();
	setStiffnessInShader(stateModifier.particleSystem().physicsComputeShader());
	setDampingInShader  (stateModifier.particleSystem().physicsComputeShader());
	stateModifier.recordChange({ StateChangeType::Physics_Pulsation,    m_values.pulsation });
	stateModifier.recordChange({ StateChangeType::Physics_DampingRatio, m_values.dampingRatio });
}

void PhysicsSettings::applyAndRecord_Pulsation(StateModifier& stateModifier) {
	m_values.computePhysicalParameters();
	setStiffnessInShader(stateModifier.particleSystem().physicsComputeShader());
	setDampingInShader  (stateModifier.particleSystem().physicsComputeShader());
	stateModifier.recordChange({ StateChangeType::Physics_Pulsation, m_values.pulsation });
}

void PhysicsSettings::applyAndRecord_DampingRatio(StateModifier& stateModifier) {
	m_values.computeDamping();
	setDampingInShader(stateModifier.particleSystem().physicsComputeShader());
	stateModifier.recordChange({ StateChangeType::Physics_DampingRatio, m_values.dampingRatio });
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