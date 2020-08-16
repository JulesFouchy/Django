#pragma once

#include "Clock/Clock.h"
#include "Recording.h"

class ConfigManager;

class RecordingManager {
public:
	RecordingManager();
	~RecordingManager() = default;

	void ImGui(ConfigManager& configManager);
	void onAction(const ActionRef& actionRef);

	inline const Clock& clock() const { return *m_clock; }
	void update();

private:
	float timeSinceStart();
	void startRecording(const ConfigRef& currentConfigAsAction);
	void stopRecording();
	Recording* currentRecording();

private:
	std::unique_ptr<Clock> m_clock;
	float m_startTime;

	bool m_bRecording = false;
	std::vector<Recording> m_recordings;
	size_t m_currRecordingIdx = -1;
};