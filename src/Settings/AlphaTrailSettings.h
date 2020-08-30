#pragma once

#include "Presets.h"

class Renderer;
class StateModifier;

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

	void ImGui(StateModifier& stateModifier);

	void setEnabled_ApplyAndRecordAll(bool bEnabled, StateModifier& stateModifier);
	void setApplyAndRecord_Enabled(bool bEnabled, StateModifier& stateModifier);
	void applyAndRecord_FixResiduals(StateModifier& stateModifier);
	void applyAndRecord_Decay(StateModifier& stateModifier);
	void applyAndRecord_Threshold(StateModifier& stateModifier);
	void applyAndRecord_MinAlpha(StateModifier& stateModifier);

	inline void setFixResiduals(bool bFixResiduals) { m_values.bFixResiduals = bFixResiduals; m_presets.setToPlaceholderSetting(); }
	inline void setDecay(float decay)               { m_values.decay = decay;                 m_presets.setToPlaceholderSetting(); }
	inline void setThreshold(float threshold)       { m_values.threshold = threshold;         m_presets.setToPlaceholderSetting(); }
	inline void setMinAlpha(float minAlpha)         { m_values.minAlpha = minAlpha;           m_presets.setToPlaceholderSetting(); }

	inline const AlphaTrailSettingsValues& getValues() const { return m_values; }
	inline void setValues(const AlphaTrailSettingsValues& values) { m_values = values; }

private:
	void applyGLBlendState();

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