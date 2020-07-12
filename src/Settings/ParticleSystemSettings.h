#pragma once

#include "Presets.h"

class ParticlesSystem;
class ConfigManager;
struct ColorSettingsValues;

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

	void ImGui(ParticlesSystem& partSystem, ConfigManager& configManager, const ColorSettingsValues& colorSettings);
	void apply(ParticlesSystem& partSystem, ConfigManager& configManager, const ColorSettingsValues& colorSettings);

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