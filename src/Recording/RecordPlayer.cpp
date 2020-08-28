#include "RecordPlayer.h"

#include "Record.h"
#include "Helper/MyImGui.h"
#include "Constants/Textures.h"
#include "RecordManager.h"
#include "PlayState_NoSelection.h"

RecordPlayer::RecordPlayer() {
	setState<PlayState_NoSelection>();
}

void RecordPlayer::update(float time, StateModifier& stateModifier) {
	m_playState->update(time, *this, stateModifier);
}

void RecordPlayer::ImGui(Record* selectedRecord, float time, StateModifier& stateModifier) {
	m_playState->ImGui(selectedRecord, time, *this, stateModifier);
}