#pragma once

#include "Presets.h"

class ShaderPipeline;

struct WindSettingsValues {
	float noiseFrequency = 0.319f;
	float minStrength = -0.048f;
	float maxStrength = 0.968f;
	float speed = 0.304f;
	float directionAngle = 2.849f;
	glm::vec2 direction = glm::vec2(-0.956f, 0.292f);
private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(noiseFrequency),
			CEREAL_NVP(minStrength),
			CEREAL_NVP(maxStrength),
			CEREAL_NVP(speed),
			CEREAL_NVP(directionAngle),
			CEREAL_NVP(direction.x),
			CEREAL_NVP(direction.y)
		);
	}
};

class WindSettings {
public:
	WindSettings();
	~WindSettings() = default;

	void apply(ShaderPipeline& physicsCompute);
	void ImGui(ShaderPipeline& physicsCompute);

	void setWindOffset(ShaderPipeline& physicsCompute, float time);

private:
	void setNoiseFrequency(ShaderPipeline& physicsCompute);
	void setMaxStrength(ShaderPipeline& physicsCompute);
	void setMinStrength(ShaderPipeline& physicsCompute);
	void setDirection(ShaderPipeline& physicsCompute);

private:
	WindSettingsValues m_values;
	Presets<WindSettingsValues> m_presets;

private:
	// Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_values),
			CEREAL_NVP(m_presets)
		);
	}
};