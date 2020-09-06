#include "Recorder.h"

#include "Helper/MyImGui.h"
#include "Constants/Textures.h"
#include "Helper/File.h"
#include "Constants/FolderPath.h"
#include "State.h"
#include "StateModifier.h"
#include "Clock/Clock.h"

Recorder::Recorder(Clock& clock)
	: m_clock(clock)
{}

Recorder::~Recorder() {
	if (isRecording())
		stop(m_clock.time());
}

void Recorder::recordChange(const StateChange& stateChange, float time) {
	if (isRecording())
		m_record.recordStateChange(stateChange, time - m_startTime);
}

void Recorder::start(const State& currentState) {
	m_record.init(currentState);
	m_startTime = currentState.timestamp;
	m_bIsRecording = true;
}

void Recorder::stop(float currentTime) {
	m_record.onRecordingStops(currentTime);
	m_bIsRecording = false;
	// Serialize record
	MyFile::CreateFolderIfDoesntExist(FolderPath::Records);
	m_record.serialize(FolderPath::Records);
}

bool Recorder::ImGui(float time, const StateModifier& stateModifier) {
	bool b = false;
	if (!isRecording()) {
		// Start recording
		if (MyImGui::ButtonWithIcon(Textures::Record(), ImVec4(1, 0, 0, 1)))
			start(stateModifier.getCurrentState());
	}
	else {
		// Stop recording
		if (MyImGui::ButtonWithIcon(Textures::Record(), ImVec4(1, 0, 0, 1), ImVec4(1, 0.2, 0.2, 1))) {
			stop(time);
			b = true;
		}
		// Show duration
		ImGui::SameLine();
		MyImGui::TimeFormatedHMS(time - m_startTime);
	}
	return b;
}