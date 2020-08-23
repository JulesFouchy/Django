#include "RecordManager.h"

#include "Actions/ActionRef.h"
#include "Clock/Clock_Realtime.h"
#include "Configurations/ConfigManager.h"
#include "Particles/ParticlesSystem.h"
#include "Helper/MyImGui.h"
#include "Helper/File.h"
#include "RecState_Idle.h"

RecordManager::RecordManager()
	: m_clock(std::make_unique<Clock_Realtime>()), m_folderPath(MyFile::RootDir + "/records")
{
	setState<RecState_Idle>();
	//
	if (MyFile::Exists(folderPath())) {
		for (auto& entry : std::filesystem::directory_iterator(folderPath())) {
			m_records.emplace_back(entry.path().string());
		}
	}
	//
	m_selectedRecordIdx = m_records.size() - 1;
}

RecordManager::~RecordManager() {
	setState<RecState_Idle>();
}

void RecordManager::setSelectedRecord(size_t idx) {
	assert(m_selectedRecordIdx != idx);
	m_selectedRecordIdx = idx;
}

void RecordManager::update(ConfigManager& configManager, ParticlesSystem& partSystem) {
	m_clock->update();
	m_recState->update(configManager, partSystem);
}

bool RecordManager::hasARecordSelected() {
	return m_selectedRecordIdx != -1;
}
Record& RecordManager::selectedRecord() {
	assert(hasARecordSelected());
	return m_records[m_selectedRecordIdx];
}

void RecordManager::ImGui(ConfigManager& configManager, ParticlesSystem& partSystem) {
	m_recState->ImGui(configManager, partSystem);
	// Records list
	for (size_t i = 0; i < m_records.size(); ++i) {
		//if (i != m_currRecordingIdx) {
			bool bIsSelectedRecording = i == m_selectedRecordIdx;
			if (ImGui::Selectable(m_records[i].name().c_str(), bIsSelectedRecording)) {
				if (!bIsSelectedRecording) {
					setSelectedRecord(i);
				}
			}
			if (ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();
				MyImGui::TimeFormatedHMS(m_records[i].totalDuration());
				ImGui::EndTooltip();
			}
		//}
	}
}