#include "RecordingManager.h"

#include "Actions/ActionRef.h"
#include "Clock/Clock_Realtime.h"
#include "Configurations/ConfigManager.h"
#include "Particles/ParticlesSystem.h"
#include "Helper/MyImGui.h"
#include "Helper/File.h"

RecordingManager::RecordingManager()
	: m_clock(std::make_unique<Clock_Realtime>())
{
	for (auto& entry : std::filesystem::directory_iterator(MyFile::RootDir + "/recordings")) {
		m_recordings.emplace_back(entry.path().string());
	}
	m_selectedRecordingIdx = m_recordings.size() - 1;
}

void RecordingManager::startRecording(const ConfigRef& currentConfigRef) {
	m_startTime = m_clock->time();
	// Create new recording
	m_recordings.emplace_back(currentConfigRef);
	m_currRecordingIdx = m_recordings.size() - 1;
}

void RecordingManager::stopRecording() {
	serializeRecording(currentlyRecording());
	m_selectedRecordingIdx = m_currRecordingIdx;
	m_currRecordingIdx = -1;
}

void RecordingManager::setSelectedRecording(size_t idx) {
	if (isRecording())
		stopRecording();
	if (isPlaying())
		stopPlaying();
	assert(m_selectedRecordingIdx != idx);
	m_selectedRecordingIdx = idx;
}

Recording& RecordingManager::currentlyRecording() {
	assert(isRecording());
	return m_recordings[m_currRecordingIdx];
}

Recording& RecordingManager::currentlyPlaying() {
	assert(isPlaying());
	return m_recordings[m_selectedRecordingIdx];
}

float RecordingManager::timeSinceStart() {
	assert(isRecording() || isPlaying());
	return m_clock->time() - m_startTime;
}

void RecordingManager::onAction(const ActionRef& actionRef) {
	if (isRecording()) {
		currentlyRecording().onAction(actionRef, timeSinceStart());
	}
}

void RecordingManager::update(ConfigManager& configManager, ParticlesSystem& partSystem) {
	m_clock->update();
	if (isPlaying())
		updatePlaying(configManager, partSystem);
}

bool RecordingManager::isRecording() {
	return m_currRecordingIdx != -1;
}

bool RecordingManager::isPlaying() {
	return m_bIsPlaying;
}

void RecordingManager::startPlaying(ConfigManager& configManager, ParticlesSystem& partSystem) {
	m_startTime = m_clock->time();
	m_bIsPlaying = true;
	if (!m_recordings[m_selectedRecordingIdx].startPlaying(configManager, partSystem))
		stopPlaying();
}

void RecordingManager::updatePlaying(ConfigManager& configManager, ParticlesSystem& partSystem) {
	if (!currentlyPlaying().updatePlaying(timeSinceStart(), configManager, partSystem))
		stopPlaying();
}

void RecordingManager::stopPlaying() {
	m_bIsPlaying = false;
}

void RecordingManager::serializeRecording(Recording& recording) {
	const std::string& folderPath = MyFile::RootDir + "/recordings";
	if (!MyFile::Exists(folderPath))
		std::filesystem::create_directory(folderPath);
	recording.serialize(folderPath);
}

void RecordingManager::ImGui(ConfigManager& configManager, ParticlesSystem& partSystem) {
	// Recording
	if (!isRecording()) {
		// Start recording
		if (ImGui::Button("Start")) {
			startRecording(configManager.getCurrentConfigRef());
		}
	}
	else {
		// Stop recording
		if (ImGui::Button("Stop")) {
			stopRecording();
		}
	}
	// Playing
	if (!isPlaying()) {
		// Start playing
		if (ImGui::Button("Play")) {
			startPlaying(configManager, partSystem);
		}
	}
	else {
		// Timeline
		float t = timeSinceStart();
		if (MyImGui::Timeline("", &t, currentlyPlaying().totalDuration())) {
			m_clock->setTime(m_startTime + t);
			if (!currentlyPlaying().setTime(t, configManager, partSystem))
				stopPlaying();
		}
		// Stop playing
		if (ImGui::Button("Stop playing")) {
			stopPlaying();
		}
	}
	// Recordings list
	for (size_t i = 0; i < m_recordings.size(); ++i) {
		if (i != m_currRecordingIdx) {
			bool bIsSelectedRecording = i == m_selectedRecordingIdx;
			if (ImGui::Selectable(m_recordings[i].name().c_str(), bIsSelectedRecording)) {
				if (!bIsSelectedRecording) {
					setSelectedRecording(i);
				}
			}
		}
	}
}