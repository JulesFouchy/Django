#pragma once

#include "Presets.h"
class StateModifier;

struct ColorSettingsValues {
	glm::vec3 bgColor = glm::vec3(0.0f, 0.0f, 0.0f);
	bool bColorModeHueGradient = true;
	// Hue gradient
	float particlesHueStart = 0.0f;
	float particlesHueEnd = 360.0f;
	float particleSaturation = 80.0f;
	float particleValue = 80.0f;
	// Color gradient
	glm::vec3 particleColorStart = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 particleColorEnd = glm::vec3(0.0f, 0.0f, 1.0f);

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(bgColor.r),
			CEREAL_NVP(bgColor.g),
			CEREAL_NVP(bgColor.b),
			CEREAL_NVP(bColorModeHueGradient),
			CEREAL_NVP(particlesHueStart),
			CEREAL_NVP(particlesHueEnd),
			CEREAL_NVP(particleSaturation),
			CEREAL_NVP(particleValue),
			CEREAL_NVP(particleColorStart.r),
			CEREAL_NVP(particleColorStart.g),
			CEREAL_NVP(particleColorStart.b),
			CEREAL_NVP(particleColorEnd.r),
			CEREAL_NVP(particleColorEnd.g),
			CEREAL_NVP(particleColorEnd.b)
		);
	}
};

class ColorSettings {
public:
	ColorSettings();
	~ColorSettings() = default;

	void apply(StateModifier& stateModifier);
	void ImGui(StateModifier& stateModifier);

	// Getters
	inline const glm::vec3& backgroundColor() { return m_values.bgColor; }
	inline const ColorSettingsValues& getValues() { return m_values; }

private:
	ColorSettingsValues m_values;
	Presets<ColorSettingsValues> m_presets;

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