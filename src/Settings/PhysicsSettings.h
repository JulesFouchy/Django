#pragma once

#include "Presets.h"

class StateModifier;
class ShaderPipeline;

struct PhysicsSettingsValues {
public:
	// High Level parameters // en.wikipedia.org/wiki/Harmonic_oscillator#Damped_harmonic_oscillator
	float pulsation = 4.472f;
	float dampingRatio = 0.335f;

	inline const float getStiffness() { return m_stiffness; }
	inline const float getDamping()   { return m_damping; }
private:
	// Actual physical parameters
	float m_stiffness;
	float m_damping;

public:
	PhysicsSettingsValues() = default;
	inline void computePhysicalParameters() {
		computeStiffness();
		computeDamping();
	}
	inline void computeStiffness() {
		m_stiffness = pulsation * pulsation;
	}
	inline void computeDamping() {
		m_damping = 2 * dampingRatio * pulsation;
	}

private:
	// Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(pulsation),
			CEREAL_NVP(dampingRatio)
		);
	}
};

class PhysicsSettings {
public:
	PhysicsSettings();
	~PhysicsSettings() = default;

	void ImGui(StateModifier& stateModifier);
	bool ImGuiPresets(StateModifier& stateModifier);
	inline void focusImGuiWindow() { m_bShouldGrabFocus = true; }

	void applyAndRecord(StateModifier& stateModifier);
	void applyAndRecord_Pulsation(StateModifier& stateModifier);
	void applyAndRecord_DampingRatio(StateModifier& stateModifier);
	void applyAndRecord_PresetName(StateModifier& stateModifier);

	inline void setPulsation(float pulsation)          { m_values.pulsation = pulsation;       m_presets.setToPlaceholderSetting(); }
	inline void setDampingRatio(float dampingRatio)    { m_values.dampingRatio = dampingRatio; m_presets.setToPlaceholderSetting(); }
	inline void setPresetName(const std::string& name) { m_presets.setPresetName(name); }

	inline const PhysicsSettingsValues& getValues() const { return m_values; }
	inline void setValues(const PhysicsSettingsValues& values) { m_values = values; m_presets.setToPlaceholderSetting(); }

private:
	void setStiffnessInShader(ShaderPipeline& physicsCompute);
	void setDampingInShader  (ShaderPipeline& physicsCompute);

private:
	PhysicsSettingsValues m_values;
	Presets<PhysicsSettingsValues> m_presets;
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