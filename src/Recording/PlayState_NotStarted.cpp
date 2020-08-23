#include "PlayState_NotStarted.h"

#include "PlayState_Play.h"
#include "PlayState_Pause.h"

#include "Helper/MyImGui.h"
#include "Constants/Textures.h"
#include "RecordPlayer.h"
#include "Record.h"

PlayState_NotStarted::PlayState_NotStarted(Record& record)
	: m_record(record)
{}

void PlayState_NotStarted::ImGui(Record* selectedRecord, float time, RecordPlayer& recordPlayer, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {
	std::function<void()> changeState = [](){};
	// Change record
	if (selectedRecord != &m_record) {
		changeState = [&](){ recordPlayer.setState<PlayState_NotStarted>(*selectedRecord); };
	}
	// Play
	if (MyImGui::ButtonWithIcon(Textures::Play())) {
		m_record.startPlaying(configManager, partSystem, recordManager);
		changeState = [&](){ recordPlayer.setState<PlayState_Play>(m_record, time); };
	}
	// Timeline
	float t = 0.0f;
	if (MyImGui::Timeline("", &t, m_record.totalDuration())) {
		m_record.setTime(t, configManager, partSystem, recordManager);
		recordPlayer.setState<PlayState_Pause>(m_record, t);
	}
	//
	changeState(); // Apply the state change at the end, otherwise we get deleted and we can't finish the ImGui
}