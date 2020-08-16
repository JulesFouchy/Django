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

private:
	State m_startState;
	std::vector<ActionTimestamp> m_actionsTimeline;
	size_t m_nextActionIdx;
};