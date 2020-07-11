#include "WindSettings.h"

#include "OpenGL/ShaderPipeline.h"

#include "Helper/MyImGui.h"

WindSettings::WindSettings()
	: m_presets("djgWind")
{}

void WindSettings::ImGui(ShaderPipeline& physicsCompute) {
	bool b = false;
	if (ImGui::SliderFloat("Frequency", &m_values.noiseFrequency, 0.0f, 1.5f)) {
		b = true;
		setNoiseFrequency(physicsCompute);
	}
	if (ImGui::SliderFloat2("Strength range", &m_values.minStrength, -1.0f, 2.0f)) {
		b = true;
		setMinStrength(physicsCompute);
		setMaxStrength(physicsCompute);
	}
	if (ImGui::SliderFloat("Speed", &m_values.speed, 0.0f, 1.5f)) {
		b = true;
	}
	if (MyImGui::AngleWheel("Direction", &m_dirValues.directionAngle)) {
		// b = true; // direction isn't actually handled by presets, so don't change b
		m_dirValues.direction = glm::vec2(cos(m_dirValues.directionAngle), sin(m_dirValues.directionAngle));
		setDirection(physicsCompute);
	}
	if (m_presets.ImGui(&m_values)) {
		apply(physicsCompute);
	}
	if (b)
		m_presets.setToPlaceholderSetting();
}

void WindSettings::apply(ShaderPipeline& physicsCompute) {
	setNoiseFrequency(physicsCompute);
	setMaxStrength(physicsCompute);
	setMinStrength(physicsCompute);
	setDirection(physicsCompute);
}

void WindSettings::setWindOffset(ShaderPipeline& physicsCompute, float time) {
	physicsCompute.setUniform2f("u_windOffset", -m_values.speed * time * m_dirValues.direction);
}
void WindSettings::setNoiseFrequency(ShaderPipeline& physicsCompute){
	physicsCompute.setUniform1f("u_windNoisePuls", m_values.noiseFrequency);
}
void WindSettings::setMaxStrength(ShaderPipeline& physicsCompute) {
	physicsCompute.setUniform1f("u_windMaxStrength", m_values.maxStrength);
}
void WindSettings::setMinStrength(ShaderPipeline& physicsCompute) {
	physicsCompute.setUniform1f("u_windMinStrength", m_values.minStrength);
}
void WindSettings::setDirection(ShaderPipeline& physicsCompute) {
	physicsCompute.setUniform1f("u_windDirectionAngle", m_dirValues.directionAngle);
}