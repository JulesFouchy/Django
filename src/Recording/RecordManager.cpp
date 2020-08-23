#include "RecordManager.h"

#include "Actions/ActionRef.h"
#include "Clock/Clock_Realtime.h"
#include "Configurations/ConfigManager.h"
#include "Particles/ParticlesSystem.h"
#include "Helper/MyImGui.h"
#include "Helper/File.h"
#include "Constants/FolderPath.h"

RecordManager::RecordManager()
	: m_clock(std::make_unique<Clock_Realtime>())
{
	// Load records
	if (MyFile::Exists(FolderPath::Records)) {
		for (auto& entry : std::filesystem::directory_iterator(FolderPath::Records)) {
			m_records.emplace_back(entry.path().string());
		}
	}
	m_selectedRecordIdx = m_records.size() - 1;
}

void RecordManager::ImGui(ConfigManager& configManager, ParticlesSystem& partSystem) {
	if (m_recorder.ImGui(configManager.getCurrentConfigRef(), m_clock->time())) // finished recording
		m_records.push_back(m_recorder.getRecord());
	m_recordPlayer.ImGui(hasARecordSelected() ? &selectedRecord() : nullptr, *m_clock, configManager, partSystem);
	ImGuiRecordsList();
}

void RecordManager::update(ConfigManager& configManager, ParticlesSystem& partSystem) {
	m_clock->update();
	m_recordPlayer.update(m_clock->time(), configManager, partSystem);
}

void RecordManager::ImGuiRecordsList() {
	for (size_t i = 0; i < m_records.size(); ++i) {
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
	}
}

bool RecordManager::hasARecordSelected() {
	return m_selectedRecordIdx != -1;
}
Record& RecordManager::selectedRecord() {
	assert(hasARecordSelected());
	return m_records[m_selectedRecordIdx];
}
void RecordManager::setSelectedRecord(size_t idx) {
	assert(m_selectedRecordIdx != idx);
	m_selectedRecordIdx = idx;
}