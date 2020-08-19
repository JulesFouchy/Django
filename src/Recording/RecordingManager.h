#pragma once

#include "Clock/Clock.h"
#include "Recording.h"

class ConfigManager;
class ParticlesSystem;

class RecordingManager {
public:
	RecordingManager();
	~RecordingManager() = default;

	void ImGui(ConfigManager& configManager, ParticlesSystem& partSystem);
	void onAction(const ActionRef& actionRef);

	inline const Clock& clock() const { return *m_clock; }
	void update(ConfigManager& configManager, ParticlesSystem& partSystem);

private:
	float timeSinceStart();
	void startRecording(const ConfigRef& currentConfigAsAction);
	void stopRecording();
	void setSelectedRecording(size_t idx);

	void startPlaying (ConfigManager& configManager, ParticlesSystem& partSystem);
	void updatePlaying(ConfigManager& configManager, ParticlesSystem& partSystem);
	void stopPlaying();

	bool isRecording();
	bool isPlaying();
	bool hasARecordSelected();
	Recording& currentlyRecording();
	Recording& currentlyPlaying();
	Recording& currentlySelected();

	void serializeRecording(Recording& recording);

private:
	std::unique_ptr<Clock> m_clock;
	float m_startTime;

	std::vector<Recording> m_recordings;
	size_t m_currRecordingIdx = -1;
	bool m_bIsPlaying = false;
	size_t m_selectedRecordingIdx = -1;

	bool m_bDraggingOnTheTimeline = false;
};