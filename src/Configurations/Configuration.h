#pragma once

class ParticleSystem;
struct ConfigParams;
struct RandomParams;

class Configuration {
public:
	Configuration() = default;
	Configuration(const std::string& mainName, const std::string& layoutName)
		: m_mainName(mainName), m_layoutName(layoutName)
	{}
	Configuration(const std::string& name)
		: Configuration(name, "")
	{}

	virtual ~Configuration() = default;

	virtual void applyTo(ParticleSystem& particleSystem, const ConfigParams& params, const RandomParams& randParams) = 0;

	inline const std::string& getName()       const { return m_mainName; }
	inline const std::string& getLayoutName() const { return m_layoutName; }

private:
	friend class ConfigManager;
	inline void setName(const std::string& name) { m_mainName = name; }
	inline void setLayoutName(const std::string& layoutName) { m_layoutName = layoutName; }

private:
	std::string m_mainName;
	std::string m_layoutName = "";
};