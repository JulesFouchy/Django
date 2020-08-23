#pragma once

#include "Clock/Clock.h"
#include "Record.h"
#include "Recorder.h"
#include "RecordPlayer.h"

class ConfigManager;
class ParticlesSystem;

class RecordManager {
public:
	RecordManager();
	~RecordManager() = default;

	void update(ConfigManager& configManager, ParticlesSystem& partSystem);
	void ImGui (ConfigManager& configManager, ParticlesSystem& partSystem);
	inline void onAction(const ActionRef& actionRef) { m_recorder.onAction(actionRef, m_clock->time()); }

	inline const Clock& clock() const { return *m_clock; }

private:
	void ImGuiRecordsList();

	bool hasARecordSelected();
	Record& selectedRecord();
	void setSelectedRecord(size_t idx);

private:
	std::unique_ptr<Clock> m_clock;

	Recorder m_recorder;
	RecordPlayer m_recordPlayer;
	std::vector<Record> m_records;
	size_t m_selectedRecordIdx = -1;
};