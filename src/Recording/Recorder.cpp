#include "Recorder.h"

#include "Helper/MyImGui.h"
#include "Constants/Textures.h"
#include "Helper/File.h"
#include "Constants/Path.h"
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
	MyFile::CreateFolderIfDoesntExist(Path::Records);
	m_record.serialize(Path::Records);
}

void Recorder::startCountdownBeforeRecording() {
	m_bIsCountingDown = true;
	m_timeToStart = std::chrono::steady_clock::now() + std::chrono::seconds(m_delayToStartRecordingInS);
}

bool Recorder::ImGui(float time, const StateModifier& stateModifier) {
	bool b = false;
	if (!isRecording()) {
		// Start recording
		if (MyImGui::ButtonWithIcon(Textures::Record(), ImVec4(1.0f, 0.0f, 0.0f, 1.0f)))
			start(stateModifier.getCurrentState());
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::Button("Start with delay")) {
				startCountdownBeforeRecording();
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			ImGui::Text("of");
			ImGui::PushID(37);
			ImGui::SliderInt("seconds.", &m_delayToStartRecordingInS, 0, 10);
			ImGui::PopID();
			ImGui::EndPopup();
		}
		// Show delay before record starts
		std::chrono::duration<float> delta = m_timeToStart - std::chrono::steady_clock::now();
		if (m_bIsCountingDown) {
			if (delta.count() < 0.f) {
				m_bIsCountingDown = false;
				start(stateModifier.getCurrentState());
			}
			else {
				ImGui::SameLine();
				ImGui::Text("Starting in %i", static_cast<int>(std::ceil(delta.count())));
			}
		}
		if (LiveMode::ShowHelpMarkers()) {
			ImGui::SameLine();
			MyImGui::HelpMarker("Will start recording. You can also right click it to get a countdown before the recording starts.");
		}
	}
	else {
		// Stop recording
		if (MyImGui::ButtonWithIcon(Textures::Record(), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.2f, 0.2f, 1.0f))) {
			stop(time);
			b = true;
		}
		// Show duration
		ImGui::SameLine();
		MyImGui::TimeFormatedHMS(time - m_startTime);
	}
	return b;
}