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

	void update(ConfigManager& configManager, ParticlesSystem& partSystem);
	void ImGui (ConfigManager& configManager, ParticlesSystem& partSystem);
	inline void onAction(const ActionRef& actionRef) { m_recState->onAction(actionRef); }

	inline const Clock& clock() const { return *m_clock; }

private:
	inline const std::string& folderPath() { return m_folderPath; }

	void ImGuiRecordsList();

	bool hasARecordSelected();
	Record& selectedRecord();
	void setSelectedRecord(size_t idx);

	template<typename T, typename... Args>
	void setState(Args&&... args) {
		m_recState = std::make_unique<T>(*this, std::forward<Args>(args)...);
	}

private:
	const std::string m_folderPath;

	std::unique_ptr<Clock> m_clock;

	std::unique_ptr<RecState> m_recState;
	std::vector<Record> m_records;
	size_t m_selectedRecordIdx = -1;

friend class RecState_Idle;
friend class RecState_Recording;
friend class RecState_Playing;
};