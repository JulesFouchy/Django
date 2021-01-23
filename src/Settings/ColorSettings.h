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

	void ImGui(StateModifier& stateModifier);
	bool ImGuiPresets(StateModifier& stateModifier);
	inline void focusImGuiWindow() { m_bShouldGrabFocus = true; }

	void applyAndRecord(StateModifier& stateModifier);
	void applyAndRecord_BackgroundColor(StateModifier& stateModifier);
	void applyAndRecord_GradientMode(StateModifier& stateModifier);
	void applyAndRecord_HueStart(StateModifier& stateModifier);
	void applyAndRecord_HueEnd(StateModifier& stateModifier);
	void applyAndRecord_Saturation(StateModifier& stateModifier);
	void applyAndRecord_Value(StateModifier& stateModifier);
	void applyAndRecord_ColorStart(StateModifier& stateModifier);
	void applyAndRecord_ColorEnd(StateModifier& stateModifier);
	void applyAndRecord_PresetName(StateModifier& stateModifier);

	inline void setBackgroundColor(const glm::vec3& col)   { m_values.bgColor = col;                      m_presets.setToPlaceholderSetting(); }
	inline void setGradientMode(bool bIsHueMode)           { m_values.bColorModeHueGradient = bIsHueMode; m_presets.setToPlaceholderSetting(); }
	inline void setHueStart(float hueStart)                { m_values.particlesHueStart = hueStart;       m_presets.setToPlaceholderSetting(); }
	inline void setHueEnd(float hueEnd)                    { m_values.particlesHueEnd = hueEnd;           m_presets.setToPlaceholderSetting(); }
	inline void setSaturation(float saturation)            { m_values.particleSaturation = saturation;    m_presets.setToPlaceholderSetting(); }
	inline void setValue(float value)                      { m_values.particleValue = value;              m_presets.setToPlaceholderSetting(); }
	inline void setColorStart(const glm::vec3& colorStart) { m_values.particleColorStart = colorStart;    m_presets.setToPlaceholderSetting(); }
	inline void setColorEnd(const glm::vec3& colorEnd)     { m_values.particleColorEnd = colorEnd;        m_presets.setToPlaceholderSetting(); }
	inline void setPresetName(const std::string& name)     { m_presets.setPresetName(name); }
	inline const std::string& getPresetName() { return m_presets.getPresetName(); }

	// Getters
	inline const glm::vec3& backgroundColor() { return m_values.bgColor; }
	inline const ColorSettingsValues& getValues() { return m_values; }
	inline void setValues(const ColorSettingsValues& values) { m_values = values; m_presets.setToPlaceholderSetting(); }

private:
	ColorSettingsValues m_values;
	Presets<ColorSettingsValues> m_presets;
	bool m_bShouldGrabFocus = false;

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