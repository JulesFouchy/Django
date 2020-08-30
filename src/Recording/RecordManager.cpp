#include "RecordManager.h"

#include "Actions/ActionRef.h"
#include "Clock/Clock_Realtime.h"
#include "StateModifier.h"
#include "Settings/SettingsManager.h"
#include "Configurations/ConfigManager.h"
#include "Helper/MyImGui.h"
#include "Helper/File.h"
#include "Constants/FolderPath.h"
#include "PlayState_NotStarted.h"
#include "PlayState_Play.h"

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

void RecordManager::ImGui(std::unique_ptr<Clock>& clock, StateModifier& stateModifier) {
	// Exporter
	if (hasARecordSelected()) {
		if (!m_exporter.isExporting()) {
			if (ImGui::Button("Export")) {
				m_exporter.startExporting(selectedRecord(), stateModifier.renderer(), clock, stateModifier.settingsManager().get().colors().backgroundColor());
				selectedRecord().startPlaying(stateModifier);
				m_recordPlayer.setState<PlayState_Play>(selectedRecord(), clock->time());
			}
		}
		else {
			if (ImGui::Button("Stop exporting")) {
				m_exporter.stopExporting(stateModifier.renderer(), clock);
				m_recordPlayer.setState<PlayState_NotStarted>(selectedRecord());
			}
		}
	}
	m_exporter.ImGui();
	ImGui::Separator();
	// Recorder
	if (m_recorder.ImGui(m_clock->time(), stateModifier)) // finished recording
		m_records.push_back(m_recorder.getRecord());
	// Record player
	m_recordPlayer.ImGui(potentialSelectedRecord(), m_clock->time(), stateModifier);
	// Records list
	ImGuiRecordsList();
}

void RecordManager::update(StateModifier& stateModifier) {
	m_clock->update();
	m_recordPlayer.update(m_clock->time(), stateModifier);
	m_exporter.update(stateModifier.renderer(), m_clock);
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