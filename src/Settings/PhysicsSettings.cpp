#include "PhysicsSettings.h"

#include "OpenGL/ShaderPipeline.h"
#include "StateModifier.h"
#include "Recording/StateChange.h"
#include "Particles/ParticlesSystem.h"

PhysicsSettings::PhysicsSettings()
	: m_presets("djgPhysics")
{}

void PhysicsSettings::ImGui(StateModifier& stateModifier) {
	if (m_bShouldGrabFocus) {
		ImGui::SetWindowFocus();
		m_bShouldGrabFocus = false;
	}
	bool b = false;
	if (ImGui::SliderFloat("Rigidity", &m_values.pulsation, 0, 20)) {
		b = true;
		applyAndRecord_Pulsation(stateModifier);
	}
	if (LiveMode::ShowHelpMarkers()) {
		ImGui::SameLine();
		MyImGui::HelpMarker(R"V0G0N(Controls how rigid the shapes are and how fast the particles move (NB : the speed of the particles is also influenced by the Damping)
)V0G0N");
	}
	if (ImGui::SliderFloat("Damping", &m_values.dampingRatio, 0, 2)) {
		b = true;
		applyAndRecord_DampingRatio(stateModifier);
	}
	if (LiveMode::ShowHelpMarkers()) {
		ImGui::SameLine();
		MyImGui::HelpMarker(R"V0G0N(Controls how much the particles oscillate. Set it to 0 for endless oscillations. After 1 there are no more oscillations, and the bigger the value the slower the particles get.
)V0G0N");
	}
	if (m_presets.ImGui(&m_values)) {
		applyAndRecord(stateModifier);
	}
	if (b)
		m_presets.setToPlaceholderSetting();
}

bool PhysicsSettings::ImGuiPresets(StateModifier& stateModifier) {
	if (m_presets.ImGuiDropdown("Physics", &m_values)) {
		applyAndRecord(stateModifier);
		return true;
	}
	return false;
}

void PhysicsSettings::applyAndRecord(StateModifier& stateModifier) {
	m_values.computePhysicalParameters();
	setStiffnessInShader(stateModifier.particleSystem().physicsComputeShader());
	setDampingInShader  (stateModifier.particleSystem().physicsComputeShader());
	stateModifier.recordChange({ StateChangeType::Physics_Pulsation,    m_values.pulsation });
	stateModifier.recordChange({ StateChangeType::Physics_DampingRatio, m_values.dampingRatio });
	applyAndRecord_PresetName(stateModifier);
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

void PhysicsSettings::applyAndRecord_PresetName(StateModifier& stateModifier) {
	stateModifier.recordChange({ StateChangeType::Physics_PresetName, m_presets.getPresetName() });
}