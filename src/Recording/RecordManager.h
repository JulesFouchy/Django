#pragma once

#include "Clock/Clock.h"
#include "Record.h"
#include "RecState.h"

class ConfigManager;
class ParticlesSystem;

class RecordManager {
public:
	RecordManager();
	~RecordManager();

	void ImGui(ConfigManager& configManager, ParticlesSystem& partSystem);
	inline void onAction(const ActionRef& actionRef) { m_recState->onAction(actionRef); }

	inline const Clock& clock() const { return *m_clock; }
	void update(ConfigManager& configManager, ParticlesSystem& partSystem);

private:
	inline const std::string& folderPath() { return m_folderPath; }

	void setSelectedRecord(size_t idx);

	bool hasARecordSelected();
	Record& selectedRecord();

	template<typename T, typename... Args>
	void setState(Args&&... args) {
		m_recState = std::make_unique<T>(*this, std::forward<Args>(args)...);
	}

private:
	const std::string m_folderPath;
	std::unique_ptr<Clock> m_clock;
	float m_startTime;

	std::unique_ptr<RecState> m_recState;

	std::vector<Record> m_records;
	size_t m_selectedRecordIdx = -1;

	bool m_bDraggingOnTheTimeline = false;

friend class RecState_Idle;
friend class RecState_Recording;
friend class RecState_Playing;
};