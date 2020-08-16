#include "Recorder.h"

#include "Actions/ActionRef.h"
#include "Clock/Clock_Realtime.h"
#include "Configurations/ConfigManager.h"

Recorder::Recorder()
	: m_clock(std::make_unique<Clock_Realtime>())
{}

void Recorder::startRecording(const ConfigRef& currentConfigRef) {
	m_bRecording = true;
	m_startTime = m_clock->time();
	m_startState.configRef = currentConfigRef;
}

void Recorder::stopRecording() {
	m_bRecording = false;
}

float Recorder::timeSinceStart() {
	assert(m_bRecording);
	return m_clock->time() - m_startTime;
}

void Recorder::onAction(const ActionRef& actionRef) {
	if (m_bRecording) {
		m_actionsTimeline.emplace_back(actionRef, timeSinceStart());
	}
}

void Recorder::update() {
	m_clock->update();
}

void Recorder::ImGui(ConfigManager& configManager) {
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