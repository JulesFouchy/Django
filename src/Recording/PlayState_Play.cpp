#include "PlayState_Play.h"

#include "PlayState_NotStarted.h"
#include "PlayState_Pause.h"

#include "Record.h"
#include "RecordPlayer.h"
#include "Helper/MyImGui.h"
#include "Constants/Textures.h"

PlayState_Play::PlayState_Play(Record& record, float startTime)
	: m_record(record), m_startTime(startTime)
{}

void PlayState_Play::update(float time, RecordPlayer& recordPlayer, StateModifier& stateModifier) {
	if (!m_record.updatePlaying(time - m_startTime, stateModifier)
		&& !m_bDraggingOnTheTimeline) // Prevent the playing from stopping just because we dragged the time cursor outside of the timeline
		recordPlayer.setState<PlayState_NotStarted>(m_record);
	//
	m_bDraggingOnTheTimeline = false; // Reset every frame. It is set by the ImGui() method before the call to update()
}

void PlayState_Play::ImGui(Record* selectedRecord, float time, RecordPlayer& recordPlayer, StateModifier& stateModifier) {
	std::function<void()> changeState = [](){};
	// Change record
	if (selectedRecord != &m_record) {
		changeState = [&](){ recordPlayer.setState<PlayState_NotStarted>(*selectedRecord); };
	}
	// Pause
	if (MyImGui::ButtonWithIcon(Textures::Pause())) {
		changeState = [&](){ recordPlayer.setState<PlayState_Pause>(m_record, time - m_startTime); };
	}
	ImGui::SameLine();
	// Stop
	if (MyImGui::ButtonWithIcon(Textures::Stop())) {
		changeState = [&](){ recordPlayer.setState<PlayState_NotStarted>(m_record); };
	}
	// Timeline
	float t = time - m_startTime;
	if (MyImGui::Timeline("MySuperbTimeline", &t, m_record.totalDuration())) {
		m_startTime = time - t;
		m_record.setTime(t, stateModifier);
		m_bDraggingOnTheTimeline = true;
	}
	//
	changeState(); // Apply the state change at the end, otherwise we get deleted and we can't finish the ImGui
}