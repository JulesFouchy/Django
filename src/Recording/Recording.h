#pragma once

#include "State.h"
#include "ActionTimestamp.h"

class ConfigManager;
class ParticlesSystem;

class Record {
public:
	Record(const ConfigRef& initialConfiguration);
	Record(const std::string& filepath);
	~Record() = default;

	void onAction(const ActionRef& actionRef, float timestamp);
	bool startPlaying(ConfigManager& configManager, ParticlesSystem& partSystem);              // Returns true iff we should keep playing the record
	bool updatePlaying(float time, ConfigManager& configManager, ParticlesSystem& partSystem); // Returns true iff we should keep playing the record

	bool setTime(float newTime, ConfigManager& configManager, ParticlesSystem& partSystem);    // Returns true iff we should keep playing the record
	inline float totalDuration() const { return m_actionsTimeline.size() > 0 ? m_actionsTimeline.back().time : 0.0f; } // TODO update me onec we have other timelines
	inline const std::string& name() const { return m_name; }

	void serialize(const std::string& folderPath);

private:
	void applyConfig(const ConfigRef& configRef, ConfigManager& configManager, ParticlesSystem& partSystem);
	void applyAction(size_t idx,                 ConfigManager& configManager, ParticlesSystem& partSystem);

	void deserialize(const std::string& filepath);

private:
	State m_startState;
	std::vector<ActionTimestamp> m_actionsTimeline;
	size_t m_nextActionIdx;
	std::string m_name;
};