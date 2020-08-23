#include "PlayState_Pause.h"

#include "PlayState_Play.h"
#include "PlayState_NotStarted.h"

#include "Helper/MyImGui.h"
#include "Constants/Textures.h"
#include "RecordPlayer.h"
#include "Record.h"
#include "Clock/Clock.h"

PlayState_Pause::PlayState_Pause(Record& record, float startTime)
	: m_record(record), m_startTime(startTime)
{}

void PlayState_Pause::ImGui(Record* selectedRecord, Clock& clock, RecordPlayer& recordPlayer, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {
	bool bStateChanged = false;
	// Change record
	if (selectedRecord != &m_record) {
		recordPlayer.setState<PlayState_NotStarted>(*selectedRecord);
		bStateChanged = true;
	}
	if (!bStateChanged) {
		// Play
		if (MyImGui::ButtonWithIcon(Textures::Play())) {
			clock.setTime(m_startTime);
			recordPlayer.setState<PlayState_Play>(m_record, m_startTime);
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
		float t = 0.0f;
		const float duration = m_record.totalDuration();
		if (MyImGui::Timeline("", &t, duration)) {
			clock.setTime(m_startTime + t);
			m_record.setTime(t, configManager, partSystem, recordManager);
		}
	}
}