#pragma once

class ShaderPipeline;

#include <glm/glm.hpp>

class WindSettings {
public:
	WindSettings() = default;
	~WindSettings() = default;

	void setUniforms(ShaderPipeline& shader);
	void ImGui_Parameters(ShaderPipeline& shader);
	void setWindOffset(ShaderPipeline& shader, float time);

private:
	void setNoiseFrequency(ShaderPipeline& shader);
	void setMaxStrength(ShaderPipeline& shader);
	void setMinStrength(ShaderPipeline& shader);
	void setDirection(ShaderPipeline& shader);

private:
	float m_noiseFrequency = 0.319f;
	float m_minStrength = -0.048f;
	float m_maxStrength = 0.968f;
	float m_speed = 0.304f;
	float m_directionAngle = 2.849f;
	glm::vec2 m_direction = glm::vec2(-0.956f, 0.292f);
};