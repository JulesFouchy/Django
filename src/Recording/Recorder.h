#pragma once

#include "State.h"
#include "ActionTimestamp.h"
#include "Clock/Clock.h"

class ConfigManager;

class Recorder {
public:
	Recorder();
	~Recorder() = default;

	void ImGui(ConfigManager& configManager);
	void onAction(const ActionRef& actionRef);

	inline const Clock& clock() const { return *m_clock; }
	void update();

private:
	float timeSinceStart();
	void startRecording(const ConfigRef& currentConfigAsAction);
	void stopRecording();

private:
	std::unique_ptr<Clock> m_clock;
	bool m_bRecording = false;

	float m_startTime;
	State m_startState;
	std::vector<ActionTimestamp> m_actionsTimeline;
};