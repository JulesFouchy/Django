#include "RecordingManager.h"

#include "Actions/ActionRef.h"
#include "Clock/Clock_Realtime.h"
#include "Configurations/ConfigManager.h"
#include "Particles/ParticlesSystem.h"

RecordingManager::RecordingManager()
	: m_clock(std::make_unique<Clock_Realtime>())
{}

void RecordingManager::startRecording(const ConfigRef& currentConfigRef) {
	m_startTime = m_clock->time();
	// Create new recording
	m_recordings.emplace_back(currentConfigRef);
	m_currRecordingIdx = m_recordings.size() - 1;
}

void RecordingManager::stopRecording() {
	m_currRecordingIdx = -1;
}

Recording* RecordingManager::currentRecording() {
	return isRecording() ? &m_recordings[m_currRecordingIdx] : nullptr;
}

float RecordingManager::timeSinceStart() {
	assert(isRecording() || isPlaying());
	return m_clock->time() - m_startTime;
}

void RecordingManager::onAction(const ActionRef& actionRef) {
	if (Recording* recording = currentRecording()) {
		recording->onAction(actionRef, timeSinceStart());
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
	return m_currPlayingIdx != -1;
}

void RecordingManager::startPlaying(ConfigManager& configManager, ParticlesSystem& partSystem) {
	m_startTime = m_clock->time();
	m_currPlayingIdx = m_selectedRecordingIdx;
	if (!m_recordings[m_currPlayingIdx].startPlaying(configManager, partSystem))
		stopPlaying();
}

void RecordingManager::updatePlaying(ConfigManager& configManager, ParticlesSystem& partSystem) {
	if (!m_recordings[m_currPlayingIdx].updatePlaying(timeSinceStart(), configManager, partSystem))
		stopPlaying();
}

void RecordingManager::stopPlaying() {
	m_currPlayingIdx = -1;
}

void RecordingManager::ImGui(ConfigManager& configManager, ParticlesSystem& partSystem) {
	// Recording
	if (!isRecording()) {
		if (ImGui::Button("Start")) {
			startRecording(configManager.getCurrentConfigRef());
		}
	}
	else {
		if (ImGui::Button("Stop")) {
			stopRecording();
		}
	}
	// Playing
	if (!isPlaying()) {
		if (ImGui::Button("Play")) {
			startPlaying(configManager, partSystem);
		}
	}
	else {
		if (ImGui::Button("Stop playing")) {
			stopPlaying();
		}
	}
	// Recordings list
	for (size_t i = 0; i < m_recordings.size(); ++i) {
		if (ImGui::Selectable(m_recordings[i].name().c_str(), i == m_selectedRecordingIdx))
			m_selectedRecordingIdx = i;
	}
}