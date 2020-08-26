#pragma once

#include "Clock/Clock.h"
#include "Record.h"
#include "Recorder.h"
#include "RecordPlayer.h"
#include "FrameExporter.h"

class ConfigManager;
class ParticlesSystem;

class RecordManager {
public:
	RecordManager();
	~RecordManager() = default;

	bool update(ConfigManager& configManager, ParticlesSystem& partSystem, Renderer& renderer); // returns true iff we stopped exporting
	bool ImGui (ConfigManager& configManager, ParticlesSystem& partSystem, Renderer& renderer, std::unique_ptr<Clock>& clock, const glm::vec3& bgColor); // returns true iff the render target has changed (i.e. we started or stopped exporting)
	inline void onAction(const ActionRef& actionRef) { m_recorder.onAction(actionRef, m_clock->time()); }

	inline const Clock& clock() const { return *m_clock; }
	inline std::unique_ptr<Clock>& clockPtrRef() { return m_clock; }
	inline FrameExporter& exporter() { return m_exporter; }
	inline Record* potentialSelectedRecord() { return hasARecordSelected() ? &selectedRecord() : nullptr; }

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

	FrameExporter m_exporter;
};