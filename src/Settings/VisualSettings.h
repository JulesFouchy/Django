#pragma once

#include "Presets.h"

struct VisualSettingsValues {
	bool bAlphaTrail = true;
	float alphaTrailDecay = 20.0f;
	glm::vec3 bgColor = glm::vec3(0.0f, 0.0f, 0.0f);

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(bAlphaTrail),
			CEREAL_NVP(alphaTrailDecay),
			CEREAL_NVP(bgColor.r),
			CEREAL_NVP(bgColor.g),
			CEREAL_NVP(bgColor.b)
		);
	}
};

class VisualSettings {
public:
	VisualSettings();
	~VisualSettings() = default;

	void apply();
	void ImGui();

	// Getters
	inline bool isAlphaTrailEnabled() { return m_values.bAlphaTrail; }
	inline float alphaTrailDecay() { return m_values.alphaTrailDecay; }
	inline const glm::vec3& backgroundColor() { return m_values.bgColor; }

private:
	static void  EnableAlphaTrail();
	static void DisableAlphaTrail();

private:
	VisualSettingsValues m_values;
	Presets<VisualSettingsValues> m_presets;

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