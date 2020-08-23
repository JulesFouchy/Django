#include "RecordManager.h"

#include "Actions/ActionRef.h"
#include "Clock/Clock_Realtime.h"
#include "Configurations/ConfigManager.h"
#include "Particles/ParticlesSystem.h"
#include "Helper/MyImGui.h"
#include "Helper/File.h"
#include "Constants/Textures.h"

RecordManager::RecordManager()
	: m_clock(std::make_unique<Clock_Realtime>())
{
	if (MyFile::Exists(MyFile::RootDir + "/records")) {
		for (auto& entry : std::filesystem::directory_iterator(MyFile::RootDir + "/records")) {
			m_records.emplace_back(entry.path().string());
		}
	}
	m_selectedRecordIdx = m_records.size() - 1;
}

void RecordManager::startRecording(const ConfigRef& currentConfigRef) {
	m_startTime = m_clock->time();
	// Create new record
	m_records.emplace_back(currentConfigRef);
	m_currRecordingIdx = m_records.size() - 1;
}

void RecordManager::stopRecording() {
	serializeRecord(currentlyRecording());
	m_selectedRecordIdx = m_currRecordingIdx;
	m_currRecordingIdx = -1;
}

void RecordManager::setSelectedRecord(size_t idx) {
	if (isRecording())
		stopRecording();
	if (isPlaying())
		stopPlaying();
	assert(m_selectedRecordIdx != idx);
	m_selectedRecordIdx = idx;
}

float RecordManager::timeSinceStart() {
	assert(isRecording() || isPlaying());
	return m_clock->time() - m_startTime;
}

void RecordManager::onAction(const ActionRef& actionRef) {
	if (isRecording()) {
		currentlyRecording().onAction(actionRef, timeSinceStart());
	}
}

void RecordManager::update(ConfigManager& configManager, ParticlesSystem& partSystem) {
	m_clock->update();
	if (isPlaying())
		updatePlaying(configManager, partSystem);
	m_bDraggingOnTheTimeline = false; // Resets every frame. It is set by the ImGui() method before the call to update()
}

bool RecordManager::isRecording() {
	return m_currRecordingIdx != -1;
}
bool RecordManager::isPlaying() {
	return m_bIsPlaying;
}
bool RecordManager::hasARecordSelected() {
	return m_selectedRecordIdx != -1;
}

Record& RecordManager::currentlyRecording() {
	assert(isRecording());
	return m_records[m_currRecordingIdx];
}
Record& RecordManager::currentlyPlaying() {
	assert(isPlaying());
	return m_records[m_selectedRecordIdx];
}
Record& RecordManager::currentlySelected() {
	assert(hasARecordSelected());
	return m_records[m_selectedRecordIdx];
}

void RecordManager::startPlaying(ConfigManager& configManager, ParticlesSystem& partSystem) {
	m_startTime = m_clock->time();
	m_bIsPlaying = true;
	if (!m_records[m_selectedRecordIdx].startPlaying(configManager, partSystem))
		stopPlaying();
}

void RecordManager::updatePlaying(ConfigManager& configManager, ParticlesSystem& partSystem) {
	if (!currentlyPlaying().updatePlaying(timeSinceStart(), configManager, partSystem)
	 && !m_bDraggingOnTheTimeline) // Prevent the playing from stopping just because we dragged the time cursor outside of the timeline
		stopPlaying();
}

void RecordManager::stopPlaying() {
	m_bIsPlaying = false;
}

void RecordManager::serializeRecord(Record& record) {
	const std::string& folderPath = MyFile::RootDir + "/records";
	if (!MyFile::Exists(folderPath))
		std::filesystem::create_directory(folderPath);
	record.serialize(folderPath);
}

void RecordManager::ImGui(ConfigManager& configManager, ParticlesSystem& partSystem) {
	// If Not Playing
	if (!isPlaying()) {
		// Recording
		if (!isRecording()) {
			// Start recording
			if (MyImGui::ButtonWithIcon(Textures::Record(), ImVec4(1, 0, 0, 1))) {
				startRecording(configManager.getCurrentConfigRef());
			}
		}
		else {
			// Stop recording
			if (MyImGui::ButtonWithIcon(Textures::Record(), ImVec4(1, 0, 0, 1), ImVec4(1, 0.2, 0.2, 1))) {
				stopRecording();
			}
		}
		ImGui::SameLine();
		// Start playing
		if (!isRecording()) {
			if (MyImGui::ButtonWithIcon(Textures::Play())) {
				startPlaying(configManager, partSystem);
			}
		}
		else {
			MyImGui::ButtonWithIconDisabled(Textures::Play(), "Cannot play while recording a clip");
		}
	}
	// If playing
	else {
		// Recording disabled
		MyImGui::ButtonWithIconDisabled(Textures::Record(), "Cannot record while playing a clip");
		ImGui::SameLine();
		// Stop playing
		if (MyImGui::ButtonWithIcon(Textures::Pause())) {
			stopPlaying();
		}
	}
	// Timeline
	if (hasARecordSelected()) {
		float t =              isPlaying() ? timeSinceStart()                   : 0.0f;
		const float duration = isPlaying() ? currentlyPlaying().totalDuration() : currentlySelected().totalDuration();
		if (MyImGui::Timeline("", &t, duration)) {
			if (!isPlaying())
				startPlaying(configManager, partSystem);
			m_clock->setTime(m_startTime + t);
			currentlyPlaying().setTime(t, configManager, partSystem);
			m_bDraggingOnTheTimeline = true;
		}
	}
	// Records list
	for (size_t i = 0; i < m_records.size(); ++i) {
		if (i != m_currRecordingIdx) {
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
}