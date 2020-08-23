#include "PlayState_Pause.h"

#include "PlayState_Play.h"
#include "PlayState_NotStarted.h"

#include "Helper/MyImGui.h"
#include "Constants/Textures.h"
#include "RecordPlayer.h"
#include "Record.h"
#include "Clock/Clock.h"

PlayState_Pause::PlayState_Pause(Record& record, float timeSinceStart)
	: m_record(record), m_timeSinceStart(timeSinceStart)
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
			recordPlayer.setState<PlayState_Play>(m_record, clock.time() - m_timeSinceStart);
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
		if (MyImGui::Timeline("", &m_timeSinceStart, m_record.totalDuration())) {
			m_record.setTime(m_timeSinceStart, configManager, partSystem, recordManager);
		}
	}
}