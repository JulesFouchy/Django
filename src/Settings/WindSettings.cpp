#include "WindSettings.h"

#include "OpenGL/ShaderPipeline.h"
#include "StateModifier.h"
#include "Helper/MyImGui.h"
#include "Particles/ParticlesSystem.h"
#include "Recording/StateChange.h"

WindSettings::WindSettings()
	: m_presets("djgWind")
{}

void WindSettings::ImGui(StateModifier& stateModifier) {
	if (m_bShouldGrabFocus) {
		ImGui::SetWindowFocus();
		m_bShouldGrabFocus = false;
	}
	if (ImGui::SliderFloat("Frequency", &m_values.noiseFrequency, 0.0f, 1.5f)) {
		m_presets.setToPlaceholderSetting();
		applyAndRecord_Frequency(stateModifier);
	}
	if (ImGui::SliderFloat2("Strength range", &m_values.minStrength, -1.0f, 2.0f)) {
		m_presets.setToPlaceholderSetting();
		applyAndRecord_MinStrength(stateModifier);
		applyAndRecord_MaxStrength(stateModifier);
	}
	if (ImGui::SliderFloat("Speed", &m_values.speed, 0.0f, 1.5f)) {
		m_presets.setToPlaceholderSetting();
		applyAndRecord_Speed(stateModifier);
	}
	if (MyImGui::AngleWheel("Direction", &m_dirValues.directionAngle)) {
		// direction isn't actually handled by presets, so don't change placeholderSettings
		m_dirValues.computeDirection();
		applyAndRecord_Direction(stateModifier);
	}
	if (m_presets.ImGui(&m_values))
		applyAndRecord(stateModifier);
}

bool WindSettings::ImGuiPresets(StateModifier& stateModifier) {
	if (m_presets.ImGuiDropdown("Wind", &m_values)) {
		applyAndRecord(stateModifier);
		return true;
	}
	return false;
}

void WindSettings::applyAndRecord(StateModifier& stateModifier) {
	applyAndRecord_Frequency(stateModifier);
	applyAndRecord_MinStrength(stateModifier);
	applyAndRecord_MaxStrength(stateModifier);
	applyAndRecord_Speed(stateModifier);
	applyAndRecord_Direction(stateModifier);
}

void WindSettings::applyAndRecord_Frequency(StateModifier& stateModifier) {
	setFrequencyInShader(stateModifier.particleSystem().physicsComputeShader());
	stateModifier.recordChange({ StateChangeType::Wind_Frequency, m_values.noiseFrequency });
}
void WindSettings::applyAndRecord_MinStrength(StateModifier& stateModifier) {
	setMinStrengthInShader(stateModifier.particleSystem().physicsComputeShader());
	stateModifier.recordChange({ StateChangeType::Wind_MinStrength, m_values.minStrength });
}
void WindSettings::applyAndRecord_MaxStrength(StateModifier& stateModifier) {
	setMaxStrengthInShader(stateModifier.particleSystem().physicsComputeShader());
	stateModifier.recordChange({ StateChangeType::Wind_MaxStrength, m_values.maxStrength });
}
void WindSettings::applyAndRecord_Speed(StateModifier& stateModifier) {
	stateModifier.recordChange({ StateChangeType::Wind_Speed, m_values.speed });
	// There is nothing to do to apply the speed, the current value is used every frame
}
void WindSettings::applyAndRecord_Direction(StateModifier& stateModifier) {
	setDirectionInShader(stateModifier.particleSystem().physicsComputeShader());
	stateModifier.recordChange({ StateChangeType::Wind_Direction, m_dirValues.directionAngle });
}

void WindSettings::setWindOffsetInShader(ShaderPipeline& physicsCompute, float time) {
	physicsCompute.setUniform2f("u_windOffset", -m_values.speed * time * m_dirValues.direction);
}
void WindSettings::setFrequencyInShader(ShaderPipeline& physicsCompute) {
	physicsCompute.bind();
	physicsCompute.setUniform1f("u_windNoisePuls", m_values.noiseFrequency);
}
void WindSettings::setMinStrengthInShader(ShaderPipeline& physicsCompute) {
	physicsCompute.bind();
	physicsCompute.setUniform1f("u_windMinStrength", m_values.minStrength);
}
void WindSettings::setMaxStrengthInShader(ShaderPipeline& physicsCompute) {
	physicsCompute.bind();
	physicsCompute.setUniform1f("u_windMaxStrength", m_values.maxStrength);
}
void WindSettings::setDirectionInShader(ShaderPipeline& physicsCompute) {
	physicsCompute.bind();
	physicsCompute.setUniform1f("u_windDirectionAngle", m_dirValues.directionAngle);
}