#include "PlayState_Pause.h"

#include "PlayState_Play.h"
#include "PlayState_NotStarted.h"

#include "Helper/MyImGui.h"
#include "Constants/Textures.h"
#include "RecordPlayer.h"
#include "Record.h"

PlayState_Pause::PlayState_Pause(Record& record, float timeSinceStart)
	: m_record(record), m_timeSinceStart(timeSinceStart)
{}

void PlayState_Pause::ImGui(Record* selectedRecord, float time, RecordPlayer& recordPlayer, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {
	std::function<void()> changeState = [](){};
	// Change record
	if (selectedRecord != &m_record) {
		changeState = [&](){ recordPlayer.setState<PlayState_NotStarted>(*selectedRecord); };
	}
	// Play
	if (MyImGui::ButtonWithIcon(Textures::Play())) {
		changeState = [&](){ recordPlayer.setState<PlayState_Play>(m_record, time - m_timeSinceStart); };
	}
	ImGui::SameLine();
	// Stop
	if (MyImGui::ButtonWithIcon(Textures::Stop())) {
		changeState = [&](){ recordPlayer.setState<PlayState_NotStarted>(m_record); };
	}
	// Timeline
	if (MyImGui::Timeline("", &m_timeSinceStart, m_record.totalDuration())) {
		m_record.setTime(m_timeSinceStart, configManager, partSystem, recordManager);
	}
	//
	changeState(); // Apply the state change at the end, otherwise we get deleted and we can't finish the ImGui
}