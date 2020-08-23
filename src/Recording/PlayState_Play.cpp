#include "PlayState_Play.h"

#include "PlayState_NotStarted.h"
#include "PlayState_Pause.h"

#include "Record.h"
#include "RecordPlayer.h"
#include "Helper/MyImGui.h"
#include "Constants/Textures.h"
#include "Clock/Clock.h"

PlayState_Play::PlayState_Play(Record& record, float startTime)
	: m_record(record), m_startTime(startTime)
{}

void PlayState_Play::update(float time, RecordPlayer& recordPlayer, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {
	if (!m_record.updatePlaying(time - m_startTime, configManager, partSystem, recordManager)
		&& !m_bDraggingOnTheTimeline) // Prevent the playing from stopping just because we dragged the time cursor outside of the timeline
		recordPlayer.setState<PlayState_NotStarted>(m_record);
	//
	m_bDraggingOnTheTimeline = false; // Reset every frame. It is set by the ImGui() method before the call to update()
}

void PlayState_Play::ImGui(Record* selectedRecord, Clock& clock, RecordPlayer& recordPlayer, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {
	bool bStateChanged = false;
	// Change record
	if (selectedRecord != &m_record) {
		recordPlayer.setState<PlayState_NotStarted>(*selectedRecord);
		bStateChanged = true;
	}
	if (!bStateChanged) {
		// Pause
		if (MyImGui::ButtonWithIcon(Textures::Pause())) {
			recordPlayer.setState<PlayState_Pause>(m_record, m_startTime);
			bStateChanged = true;
		}
	}
	if (!bStateChanged) {
		ImGui::SameLine();
		// Stop
		if (MyImGui::ButtonWithIcon(Textures::Stop())) {
			recordPlayer.setState<PlayState_NotStarted>(m_record);
			bStateChanged = true;
		}
	}
	if (!bStateChanged) {
		// Timeline
		float t = clock.time() - m_startTime;
		const float duration = m_record.totalDuration();
		if (MyImGui::Timeline("", &t, duration)) {
			clock.setTime(m_startTime + t);
			m_record.setTime(t, configManager, partSystem, recordManager);
			m_bDraggingOnTheTimeline = true;
		}
	}
}