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
	// Returns true iff we should keep playing the recording
	bool startPlaying(ConfigManager& configManager, ParticlesSystem& partSystem);
	// Returns true iff we should keep playing the recording
	bool updatePlaying(float time, ConfigManager& configManager, ParticlesSystem& partSystem);

	inline float totalDuration() const { return m_actionsTimeline.back().time; } // TODO update me onec we have other timelines
	inline const std::string& name() const { return m_name; }

private:
	State m_startState;
	std::vector<ActionTimestamp> m_actionsTimeline;
	size_t m_nextActionIdx;
	std::string m_name;
};