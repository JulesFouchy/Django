#include "Recorder.h"

#include "Helper/MyImGui.h"
#include "Constants/Textures.h"
#include "Helper/File.h"
#include "Constants/FolderPath.h"

Recorder::~Recorder() {
	if (isRecording())
		stop();
}

void Recorder::onStateChange(const StateChange& stateChange, float time) {
	if (isRecording())
		m_record.recordStateChange(stateChange, time - m_startTime);
}

void Recorder::start(const ConfigRef& currentConfigRef, float time) {
	m_record.init(time, currentConfigRef);
	m_startTime = time;
	m_bIsRecording = true;
}

void Recorder::stop() {
	m_bIsRecording = false;
	// Serialize record
	MyFile::CreateFolderIfDoesntExist(FolderPath::Records);
	m_record.serialize(FolderPath::Records);
}

bool Recorder::ImGui(const ConfigRef& currentConfigRef, float time) {
	bool b = false;
	if (!isRecording()) {
		// Start recording
		if (MyImGui::ButtonWithIcon(Textures::Record(), ImVec4(1, 0, 0, 1)))
			start(currentConfigRef, time);
	}
	else {
		// Stop recording
		if (MyImGui::ButtonWithIcon(Textures::Record(), ImVec4(1, 0, 0, 1), ImVec4(1, 0.2, 0.2, 1))) {
			stop();
			b = true;
		}
		// Show duration
		ImGui::SameLine();
		MyImGui::TimeFormatedHMS(time - m_startTime);
	}
	return b;
}