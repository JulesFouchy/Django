#include "PlayState_NotStarted.h"

#include "PlayState_Play.h"
#include "PlayState_Pause.h"

#include "Helper/MyImGui.h"
#include "Constants/Textures.h"
#include "RecordPlayer.h"
#include "Record.h"

PlayState_NotStarted::PlayState_NotStarted(Record& record, bool bStartJustAfterThat)
	: m_record(record), m_bStartJustAfterThat(bStartJustAfterThat)
{}

void PlayState_NotStarted::ImGui(Record* selectedRecord, float time, RecordPlayer& recordPlayer, StateModifier& stateModifier) {
	std::function<void()> changeState = [](){};
	// Change record
	if (selectedRecord != &m_record) {
		changeState = [&](){ recordPlayer.setState<PlayState_NotStarted>(*selectedRecord); };
	}
	// Play
	if (!m_bStartJustAfterThat) {
		if (MyImGui::ButtonWithIcon(Textures::Play())) {
			m_record.startPlaying(stateModifier);
			changeState = [&]() { recordPlayer.setState<PlayState_Play>(m_record, time); };
		}
	}
	else {
		// To prevent icons flashing for a fraction of a second we act as if we are already playing
		MyImGui::ButtonWithIcon(Textures::Pause()); ImGui::SameLine();
		MyImGui::ButtonWithIcon(Textures::Stop());
		// Start playing
		m_record.startPlaying(stateModifier);
		changeState = [&]() { recordPlayer.setState<PlayState_Play>(m_record, time); };
	}
	// Timeline
	float t = 0.0f;
	if (MyImGui::Timeline("MySuperbTimeline", &t, m_record.totalDuration(), false)) {
		m_record.setTime(t, stateModifier);
		recordPlayer.setState<PlayState_Pause>(m_record, t);
	}
	//
	changeState(); // Apply the state change at the end, otherwise we get deleted and we can't finish the ImGui
}