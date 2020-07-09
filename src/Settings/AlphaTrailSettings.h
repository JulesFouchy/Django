#pragma once

#include "Presets.h"

struct AlphaTrailSettingsValues {
	bool bAlphaTrail = true;
	float alphaTrailDecay = 20.0f;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(bAlphaTrail),
			CEREAL_NVP(alphaTrailDecay)
		);
	}
};

class AlphaTrailSettings {
public:
	AlphaTrailSettings();
	~AlphaTrailSettings() = default;

	void apply();
	void ImGui();

	// Getters
	inline bool isEnabled() { return m_values.bAlphaTrail; }
	inline float alphaTrailDecay() { return m_values.alphaTrailDecay; }

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