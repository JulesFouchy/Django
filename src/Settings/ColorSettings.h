#pragma once

#include "Presets.h"

struct ColorSettingsValues {
	glm::vec3 bgColor = glm::vec3(0.0f, 0.0f, 0.0f);
	float particlesHueStart = 0.0f;
	float particlesHueEnd = 360.0f;

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
			CEREAL_NVP(particlesHueStart),
			CEREAL_NVP(particlesHueEnd)
		);
	}
};

class ColorSettings {
public:
	ColorSettings();
	~ColorSettings() = default;

	void apply();
	void ImGui();

	// Getters
	inline const glm::vec3& backgroundColor() { return m_values.bgColor; }

private:
	void applyParticlesColors();

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