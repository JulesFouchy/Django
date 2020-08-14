#pragma once

class ParticlesSystem;
struct ConfigParams;
struct RandomParams;

class Configuration {
public:
	Configuration() = default;
	Configuration(const std::string& name)
		: m_name(name)
	{}

	virtual ~Configuration() = default;

	virtual void applyTo(ParticlesSystem& particlesSystem, const ConfigParams& params, const RandomParams& randParams) = 0;

	inline const std::string& getName() const { return m_name; }

private:
	friend class ConfigManager;
	inline void setName(const std::string& name) { m_name = name; }

private:
	std::string m_name;
};