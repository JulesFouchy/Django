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
	bool b = false;
	if (ImGui::SliderFloat("Frequency", &m_values.noiseFrequency, 0.0f, 1.5f)) {
		b = true;
		setNoiseFrequency(stateModifier);
	}
	if (ImGui::SliderFloat2("Strength range", &m_values.minStrength, -1.0f, 2.0f)) {
		b = true;
		setMinStrength(stateModifier);
		setMaxStrength(stateModifier);
	}
	if (ImGui::SliderFloat("Speed", &m_values.speed, 0.0f, 1.5f)) {
		b = true;
	}
	if (MyImGui::AngleWheel("Direction", &m_dirValues.directionAngle)) {
		// b = true; // direction isn't actually handled by presets, so don't change b
		m_dirValues.direction = glm::vec2(cos(m_dirValues.directionAngle), sin(m_dirValues.directionAngle));
		setDirection(stateModifier);
	}
	if (m_presets.ImGui(&m_values)) {
		apply(stateModifier);
	}
	if (b)
		m_presets.setToPlaceholderSetting();
}

void WindSettings::apply(StateModifier& stateModifier) {
	setNoiseFrequency(stateModifier);
	setMaxStrength(stateModifier);
	setMinStrength(stateModifier);
	setDirection(stateModifier);
}

void WindSettings::setWindOffset(ShaderPipeline& physicsCompute, float time) {
	physicsCompute.setUniform2f("u_windOffset", -m_values.speed * time * m_dirValues.direction);
}
void WindSettings::setNoiseFrequency(StateModifier& stateModifier){
	stateModifier.particleSystem().physicsComputeShader().setUniform1f("u_windNoisePuls", m_values.noiseFrequency);
	stateModifier.recordChange({ StateChangeType::Wind_NoiseFrequency, m_values.noiseFrequency });
}
void WindSettings::setMaxStrength(StateModifier& stateModifier) {
	stateModifier.particleSystem().physicsComputeShader().setUniform1f("u_windMaxStrength", m_values.maxStrength);
	stateModifier.recordChange({ StateChangeType::Wind_MaxStrength, m_values.maxStrength });
}
void WindSettings::setMinStrength(StateModifier& stateModifier) {
	stateModifier.particleSystem().physicsComputeShader().setUniform1f("u_windMinStrength", m_values.minStrength);
	stateModifier.recordChange({ StateChangeType::Wind_MinStrength, m_values.minStrength });
}
void WindSettings::setDirection(StateModifier& stateModifier) {
	stateModifier.particleSystem().physicsComputeShader().setUniform1f("u_windDirectionAngle", m_dirValues.directionAngle);
	stateModifier.recordChange({ StateChangeType::Wind_DirectionAngle, m_dirValues.directionAngle });
}