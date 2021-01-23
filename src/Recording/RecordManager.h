#pragma once

#include "Clock/Clock.h"
#include "Record.h"
#include "Recorder.h"
#include "RecordPlayer.h"
#include "FrameExporter.h"
#include "OpenGL/ComputeShader.h"

class StateModifier;

class RecordManager {
public:
	RecordManager();
	~RecordManager() = default;

	void update(StateModifier& stateModifier);
	void ImGui (std::unique_ptr<Clock>& clock, StateModifier& stateModifier);
	inline void recordChange(const StateChange& stateChange) { m_recorder.recordChange(stateChange, m_clock->time()); }

	inline const Clock& clock() const { return *m_clock; }
	inline std::unique_ptr<Clock>& clockPtrRef() { return m_clock; }
	inline FrameExporter& exporter() { return m_exporter; }
	inline Record* potentialSelectedRecord() { return hasARecordSelected() ? &selectedRecord() : nullptr; }

private:
	void ImGuiRecordsList();

	bool hasARecordSelected();
	Record& selectedRecord();
	void setSelectedRecord(size_t idx);
	void deselectRecord();
	void deleteRecord(size_t idx);
	void validateRecordRenaming();

private:
	std::unique_ptr<Clock> m_clock;

	Recorder m_recorder;
	RecordPlayer m_recordPlayer;
	std::vector<Record> m_records;
	size_t m_selectedRecordIdx = -1;
	size_t m_recordBeingRenamedIdx = -1;
	std::string m_newRecordName;

	FrameExporter m_exporter;
	ComputeShader m_resetParticlesPosAndSpeedShader;
};