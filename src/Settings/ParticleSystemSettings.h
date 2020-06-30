#pragma once

class ParticlesSystem;
class Configuration;

class ParticleSystemSettings {
public:
	ParticleSystemSettings() = default;
	~ParticleSystemSettings() = default;

	void ImGui(ParticlesSystem& partSystem, Configuration& currentConfiguration);
	void apply(ParticlesSystem& partSystem, Configuration& currentConfiguration);

private:
	unsigned int m_nbParticles = 1000000;
	float m_particleRadiusRelToHeight = 0.002f;

	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_nbParticles),
			CEREAL_NVP(m_particleRadiusRelToHeight)
		);
	}
};