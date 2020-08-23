#include "RecordPlayer.h"

#include "Record.h"
#include "Helper/MyImGui.h"
#include "Constants/Textures.h"
#include "RecordManager.h"
#include "PlayState_NoSelection.h"

RecordPlayer::RecordPlayer() {
	setState<PlayState_NoSelection>();
}

void RecordPlayer::update(float time, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {
	m_playState->update(time, *this, configManager, partSystem, recordManager);
}

void RecordPlayer::ImGui(Record* selectedRecord, float time, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {
	m_playState->ImGui(selectedRecord, time, *this, configManager, partSystem, recordManager);
}