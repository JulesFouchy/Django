#pragma once

#include "State.h"
#include "ActionTimestamp.h"

class ConfigManager;
class ParticlesSystem;

class Recording {
public:
	Recording(const ConfigRef& initialConfiguration);
	~Recording() = default;

	void onAction(const ActionRef& actionRef, float timestamp);
	bool startPlaying(ConfigManager& configManager, ParticlesSystem& partSystem);              // Returns true iff we should keep playing the recording
	bool updatePlaying(float time, ConfigManager& configManager, ParticlesSystem& partSystem); // Returns true iff we should keep playing the recording

	bool setTime(float newTime, ConfigManager& configManager, ParticlesSystem& partSystem); // Returns true iff we should keep playing the recording
	inline float totalDuration() const { return m_actionsTimeline.back().time; } // TODO update me onec we have other timelines
	inline const std::string& name() const { return m_name; }

private:
	void applyConfig(const ConfigRef& configRef, ConfigManager& configManager, ParticlesSystem& partSystem);
	void applyAction(size_t idx,                 ConfigManager& configManager, ParticlesSystem& partSystem);

private:
	State m_startState;
	std::vector<ActionTimestamp> m_actionsTimeline;
	size_t m_nextActionIdx;
	std::string m_name;
};