#include "WindSettings.h"

#include "OpenGL/ShaderPipeline.h"

void WindSettings::ImGui(ShaderPipeline& physicsCompute) {
	if (ImGui::SliderFloat("Frequency", &m_noiseFrequency, 0.0f, 1.5f))
		setNoiseFrequency(physicsCompute);
	if (ImGui::SliderFloat2("Strength range", &m_minStrength, -1.0f, 2.0f)) {
		setMinStrength(physicsCompute);
		setMaxStrength(physicsCompute);
	}
	ImGui::SliderFloat("Speed", &m_speed, 0.0f, 1.5f);
}

void WindSettings::apply(ShaderPipeline& physicsCompute) {
	setNoiseFrequency(physicsCompute);
	setMaxStrength(physicsCompute);
	setMinStrength(physicsCompute);
	setDirection(physicsCompute);
}

void WindSettings::setWindOffset(ShaderPipeline& shader, float time) {
	shader.setUniform2f("u_windOffset", - m_speed * time * m_direction);
}
void WindSettings::setNoiseFrequency(ShaderPipeline& shader){
	shader.setUniform1f("u_windNoisePuls", m_noiseFrequency);
}
void WindSettings::setMaxStrength(ShaderPipeline& shader) {
	shader.setUniform1f("u_windMaxStrength", m_maxStrength);
}
void WindSettings::setMinStrength(ShaderPipeline& shader) {
	shader.setUniform1f("u_windMinStrength", m_minStrength);
}
void WindSettings::setDirection(ShaderPipeline& shader) {
	shader.setUniform1f("u_windDirectionAngle", m_directionAngle);
}