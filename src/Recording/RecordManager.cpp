#include "RecordManager.h"

#include "Actions/ActionRef.h"
#include "Clock/Clock_Realtime.h"
#include "StateModifier.h"
#include "Settings/SettingsManager.h"
#include "Particles/ParticlesSystem.h"
#include "Configurations/ConfigManager.h"
#include "Helper/MyImGui.h"
#include "Helper/File.h"
#include "Constants/Path.h"
#include "PlayState_NotStarted.h"
#include "PlayState_Play.h"
#include "PlayState_NoSelection.h"
#include "Viewports/Viewports.h"
#include <Boxer/boxer.h>

RecordManager::RecordManager()
	: m_clock(std::make_unique<Clock_Realtime>()), m_recorder(*m_clock), m_resetParticlesPosAndSpeedShader("internal-shaders/resetParticlePositionAndSpeed.comp")
{
	// Load records
	if (MyFile::Exists(Path::Records)) {
		for (auto& entry : std::filesystem::directory_iterator(Path::Records)) {
			m_records.emplace_back(entry.path().string());
		}
	}
	m_selectedRecordIdx = m_records.size() - 1;
}

void RecordManager::ImGui(std::unique_ptr<Clock>& clock, StateModifier& stateModifier) {
	// Exporter
	if (LiveMode::IsOff()) {
		m_exporter.ImGui();
		if (hasARecordSelected()) {
			if (!Viewports::IsExporting()) {
				if (ImGui::Button("Export")) {
					if (m_exporter.startExporting(selectedRecord(), stateModifier.renderer(), clock)) {
						// Play record
						selectedRecord().startPlaying(stateModifier);
						m_recordPlayer.setState<PlayState_Play>(selectedRecord(), clock->time());
						// Reset particles' positions and speeds
						unsigned int nbParticles = stateModifier.particleSystem().getNbParticles();
						m_resetParticlesPosAndSpeedShader.get().bind();
						m_resetParticlesPosAndSpeedShader.get().setUniform1i("u_NbOfParticles", nbParticles);
						m_resetParticlesPosAndSpeedShader.compute(nbParticles);
					}
				}
			}
			else {
				if (ImGui::Button("Stop exporting")) {
					m_exporter.stopExporting(stateModifier.renderer(), clock);
					m_recordPlayer.setState<PlayState_NotStarted>(selectedRecord());
				}
			}
		}
	}
	if (!Viewports::IsExporting()) {
		if (LiveMode::IsOff())
			ImGui::Separator();
		// Recorder
		if (m_recorder.ImGui(m_clock->time(), stateModifier)) // finished recording
			m_records.push_back(m_recorder.getRecord());
		// Record player
		m_recordPlayer.ImGui(potentialSelectedRecord(), m_clock->time(), stateModifier);
		// Records list
		ImGuiRecordsList();
	}
}

void RecordManager::update(StateModifier& stateModifier) {
	m_clock->update();
	m_recordPlayer.update(m_clock->time(), stateModifier);
	m_exporter.update(stateModifier.renderer(), m_clock);
}

void RecordManager::ImGuiRecordsList() {
	size_t markedForDelete = -1;
	bool bAllContextMenusAreClosed = true;
	for (size_t i = 0; i < m_records.size(); ++i) {
		bool bIsSelectedRecording = i == m_selectedRecordIdx;
		// Selectable record
		if (ImGui::Selectable(m_records[i].name().c_str(), bIsSelectedRecording)) {
			if (!bIsSelectedRecording) {
				setSelectedRecord(i);
			}
		}
		// Duration tooltip
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			MyImGui::TimeFormatedHMS(m_records[i].totalDuration());
			ImGui::EndTooltip();
		}
		// Context menu
		if (LiveMode::IsOff()) {
			if (ImGui::BeginPopupContextItem(m_records[i].name().c_str())) {
				bAllContextMenusAreClosed = false;
				if (m_recordBeingRenamedIdx != i) {
					// Buttons
					if (ImGui::Button("Rename")) {
						m_recordBeingRenamedIdx = i;
						m_newRecordName = m_records[i].name();
					}
					if (ImGui::Button("Delete")) {
						if (boxer::show(("\"" + m_records[i].name() + "\" will be deleted. Are you sure ?").c_str(), "Delete", boxer::Style::Warning, boxer::Buttons::YesNo) == boxer::Selection::Yes) {
							markedForDelete = i;
						}
					}
				}
				else {
					// Rename input
					ImGui::PushID(46824987249);
					ImGui::InputText("", &m_newRecordName);
					ImGui::PopID();
				}
				ImGui::EndPopup();
			}
		}
	}
	if (markedForDelete != -1) {
		deleteRecord(markedForDelete);
	}
	if (bAllContextMenusAreClosed)
		validateRecordRenaming();
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

void RecordManager::deleteRecord(size_t idx) {
	std::filesystem::remove(Path::Records + "/" + m_records[idx].name() + ".djgRecord");
	m_records.erase(idx + m_records.begin());
	if (idx == m_selectedRecordIdx) {
		setSelectedRecord(-1);
		m_recordPlayer.setState<PlayState_NoSelection>();
	}
	else if (hasARecordSelected() && m_selectedRecordIdx > idx) {
		m_selectedRecordIdx--;
	}
}

void RecordManager::validateRecordRenaming() {
	if (m_recordBeingRenamedIdx != -1) {
		while (MyFile::Exists(Path::Records + "/" + m_newRecordName + ".djgRecord"))
			m_newRecordName += "_";
		m_records[m_recordBeingRenamedIdx].setName(m_newRecordName);
		m_recordBeingRenamedIdx = -1;
	}
}