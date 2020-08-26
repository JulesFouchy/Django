#pragma once

#include "Presets.h"

class Renderer;

struct AlphaTrailSettingsValues {
	bool bEnabled = true;
	bool bFixResiduals = false;
	float decay = 20.0f;
	float threshold = 0.233;
	float minAlpha = 0.112;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(bEnabled),
			CEREAL_NVP(bFixResiduals),
			CEREAL_NVP(decay),
			CEREAL_NVP(threshold),
			CEREAL_NVP(minAlpha)
		);
	}
};

class AlphaTrailSettings {
public:
	AlphaTrailSettings();
	~AlphaTrailSettings() = default;

	void apply();
	void ImGui(const glm::vec3& bgColor, Renderer& renderer);

	const AlphaTrailSettingsValues& getValues() const { return m_values; }

private:
	AlphaTrailSettingsValues m_values;
	Presets<AlphaTrailSettingsValues> m_presets;

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