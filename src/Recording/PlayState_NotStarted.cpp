#include "PlayState_NotStarted.h"

#include "PlayState_Play.h"

#include "Helper/MyImGui.h"
#include "Constants/Textures.h"
#include "RecordPlayer.h"
#include "Record.h"

PlayState_NotStarted::PlayState_NotStarted(Record& record)
	: m_record(record)
{}

void PlayState_NotStarted::ImGui(Record* selectedRecord, float time, RecordPlayer& recordPlayer, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {
	bool bStateChanged = false;
	// Change record
	if (selectedRecord != &m_record) {
		recordPlayer.setState<PlayState_NotStarted>(*selectedRecord);
		bStateChanged = true;
	}
	if (!bStateChanged) {
		// Play
		if (MyImGui::ButtonWithIcon(Textures::Play())) {
			m_record.startPlaying(configManager, partSystem, recordManager);
			recordPlayer.setState<PlayState_Play>(m_record, time);
		}
	}
}