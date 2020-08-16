#include "RecordingManager.h"

#include "Actions/ActionRef.h"
#include "Clock/Clock_Realtime.h"
#include "Configurations/ConfigManager.h"

RecordingManager::RecordingManager()
	: m_clock(std::make_unique<Clock_Realtime>())
{}

void RecordingManager::startRecording(const ConfigRef& currentConfigRef) {
	m_bRecording = true;
	m_startTime = m_clock->time();
	// Create new recording
	m_recordings.emplace_back(currentConfigRef);
	m_currRecordingIdx = m_recordings.size() - 1;
}

void RecordingManager::stopRecording() {
	m_bRecording = false;
}

Recording* RecordingManager::currentRecording() {
	return m_bRecording ? &m_recordings[m_currRecordingIdx] : nullptr;
}

float RecordingManager::timeSinceStart() {
	assert(m_bRecording);
	return m_clock->time() - m_startTime;
}

void RecordingManager::onAction(const ActionRef& actionRef) {
	if (Recording* recording = currentRecording()) {
		recording->onAction(actionRef, timeSinceStart());
	}
}

void RecordingManager::update() {
	m_clock->update();
}

void RecordingManager::ImGui(ConfigManager& configManager) {
	if (!m_bRecording) {
		if (ImGui::Button("Start")) {
			startRecording(configManager.getCurrentConfigRef());
		}
	}
	else {
		if (ImGui::Button("Stop")) {
			stopRecording();
		}
	}
}