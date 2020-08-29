#pragma once

#include "Presets.h"

class StateModifier;

struct ParticleSystemSettingsValues {
	unsigned int nbParticles = 100000;
	float particleRadiusRelToHeight = 0.002f;

	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
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

	void applyAndRecord(StateModifier& stateModifier);
	void applyAndRecord_NbParticles(StateModifier& stateModifier);
	void applyAndRecord_ParticleRadius(StateModifier& stateModifier);

	inline void setNbParticles(unsigned int nbParticles) { m_values.nbParticles = nbParticles;          m_presets.setToPlaceholderSetting(); }
	inline void setRadius(float radius)                  { m_values.particleRadiusRelToHeight = radius; m_presets.setToPlaceholderSetting(); }

	inline unsigned int nbParticles() { return m_values.nbParticles; }

private:
	ParticleSystemSettingsValues m_values;
	Presets<ParticleSystemSettingsValues> m_presets;

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