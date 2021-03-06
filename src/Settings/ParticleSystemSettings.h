#pragma once

#include "Presets.h"

class StateModifier;

struct ParticleSystemSettingsValues {
	unsigned int nbParticles = 100000;
	float particleRadiusRelToHeight = 0.002f;

	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive, std::uint32_t const version)
	{
		archive(
			CEREAL_NVP(nbParticles),
			CEREAL_NVP(particleRadiusRelToHeight)
		);
	}
};

class ParticleSystemSettings {
public:
	ParticleSystemSettings();
	~ParticleSystemSettings() = default;

	void ImGui(StateModifier& stateModifier);
	bool ImGuiPresets(StateModifier& stateModifier);
	inline void focusImGuiWindow() { m_bShouldGrabFocus = true; }

	void applyAndRecord(StateModifier& stateModifier);
	void applyAndRecord_NbParticles(StateModifier& stateModifier);
	void applyAndRecord_ParticleRadius(StateModifier& stateModifier);
	void applyAndRecord_PresetName(StateModifier& stateModifier);

	inline void setNbParticles(unsigned int nbParticles) { m_values.nbParticles = nbParticles;          m_presets.setToPlaceholderSetting(); }
	inline void setRadius(float radius)                  { m_values.particleRadiusRelToHeight = radius; m_presets.setToPlaceholderSetting(); }
	inline void setPresetName(const std::string& name)   { m_presets.setPresetName(name); }
	inline const std::string& getPresetName() { return m_presets.getPresetName(); }

	inline unsigned int nbParticles() { return m_values.nbParticles; }
	inline const ParticleSystemSettingsValues& getValues() const { return m_values; }
	inline void setValues(const ParticleSystemSettingsValues& values) { m_values = values; m_presets.setToPlaceholderSetting(); }

private:
	ParticleSystemSettingsValues m_values;
	Presets<ParticleSystemSettingsValues> m_presets;
	bool m_bShouldGrabFocus = false;

private:
	// Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive, std::uint32_t const version)
	{
		archive(
			CEREAL_NVP(m_values),
			CEREAL_NVP(m_presets)
		);
	}
};