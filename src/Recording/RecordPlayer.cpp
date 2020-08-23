#include "RecordPlayer.h"

#include "Record.h"
#include "Helper/MyImGui.h"
#include "Constants/Textures.h"
#include "Clock/Clock.h"

void RecordPlayer::update(float time, ConfigManager& configManager, ParticlesSystem& partSystem) {
	if (isPlaying()) {
		if (!m_record->updatePlaying(time - m_startTime, configManager, partSystem)
		&& !m_bDraggingOnTheTimeline) // Prevent the playing from stopping just because we dragged the time cursor outside of the timeline
			stop();
	}
	//
	m_bDraggingOnTheTimeline = false; // Reset every frame. It is set by the ImGui() method before the call to update()
}

void RecordPlayer::ImGui(Record* selectedRecord, Clock& clock, ConfigManager& configManager, ParticlesSystem& partSystem) {
	bool bStateChanged = false;
	if (!isPlaying()) {
		// Start playing
		if (selectedRecord) {
			if (MyImGui::ButtonWithIcon(Textures::Play())) {
				start(selectedRecord, clock.time(), configManager, partSystem);
				bStateChanged = true;
			}
		}
		else {
			MyImGui::ButtonWithIconDisabled(Textures::Play(), "No record selected");
		}
	}
	else {
		// Stop playing
		if (MyImGui::ButtonWithIcon(Textures::Pause())) {
			stop();
			bStateChanged = true;
		}
		if (!bStateChanged) {
			// Timeline
			float t = clock.time() - m_startTime;
			const float duration = m_record->totalDuration();
			if (MyImGui::Timeline("", &t, duration)) {
				clock.setTime(m_startTime + t);
				m_record->setTime(t, configManager, partSystem);
				m_bDraggingOnTheTimeline = true;
			}
		}
	}
}

void RecordPlayer::start(Record* record, float time, ConfigManager& configManager, ParticlesSystem& partSystem) {
	if (!record->startPlaying(configManager, partSystem))
		stop();
	else {
		m_record = record;
		m_startTime = time;
	}
}

void RecordPlayer::stop() {
	m_record = nullptr;
}