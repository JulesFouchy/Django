#pragma once

#include "Presets.h"
class ParticlesSystem;

struct ColorSettingsValues {
	glm::vec3 bgColor = glm::vec3(0.0f, 0.0f, 0.0f);
	float particlesHueStart = 0.0f;
	float particlesHueEnd = 360.0f;
	float particleSaturation = 80.0f;
	float particleValue = 80.0f;

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
			CEREAL_NVP(particlesHueEnd),
			CEREAL_NVP(particleSaturation),
			CEREAL_NVP(particleValue)
		);
	}
};

class ColorSettings {
public:
	ColorSettings();
	~ColorSettings() = default;

	void apply(ParticlesSystem& partSystem);
	void ImGui(ParticlesSystem& partSystem);

	// Getters
	inline const glm::vec3& backgroundColor() { return m_values.bgColor; }
	inline const ColorSettingsValues& getValues() { return m_values; }

private:
	void applyParticlesColors(ParticlesSystem& partSystem);

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