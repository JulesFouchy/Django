#include "RecordPlayer.h"

#include "Record.h"
#include "Helper/MyImGui.h"
#include "Constants/Textures.h"
#include "Clock/Clock.h"
#include "RecordManager.h"
#include "PlayState_NoSelection.h"

RecordPlayer::RecordPlayer() {
	setState<PlayState_NoSelection>();
}

void RecordPlayer::update(float time, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {
	m_playState->update(time, *this, configManager, partSystem, recordManager);
}

void RecordPlayer::ImGui(Record* selectedRecord, Clock& clock, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {
	m_playState->ImGui(selectedRecord, clock, *this, configManager, partSystem, recordManager);
}