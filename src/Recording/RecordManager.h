#pragma once

#include "Clock/Clock.h"
#include "Record.h"

class ConfigManager;
class ParticlesSystem;

class RecordManager {
public:
	RecordManager();
	~RecordManager() = default;

	void ImGui(ConfigManager& configManager, ParticlesSystem& partSystem);
	void onAction(const ActionRef& actionRef);

	inline const Clock& clock() const { return *m_clock; }
	void update(ConfigManager& configManager, ParticlesSystem& partSystem);

private:
	float timeSinceStart();

	void startRecording(const ConfigRef& currentConfigAsAction);
	void stopRecording();

	void setSelectedRecord(size_t idx);

	void startPlaying (ConfigManager& configManager, ParticlesSystem& partSystem);
	void updatePlaying(ConfigManager& configManager, ParticlesSystem& partSystem);
	void stopPlaying();

	bool isRecording();
	bool isPlaying();
	bool hasARecordSelected();
	Record& currentlyRecording();
	Record& currentlyPlaying();
	Record& currentlySelected();

	void serializeRecord(Record& record);

private:
	std::unique_ptr<Clock> m_clock;
	float m_startTime;

	std::vector<Record> m_records;
	size_t m_currRecordingIdx = -1;
	bool m_bIsPlaying = false;
	size_t m_selectedRecordIdx = -1;

	bool m_bDraggingOnTheTimeline = false;
};