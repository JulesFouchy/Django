#include "PlayState_NoSelection.h"

#include "PlayState_NotStarted.h"

#include "RecordPlayer.h"
#include "Helper/MyImGui.h"
#include "Constants/Textures.h"

void PlayState_NoSelection::ImGui(Record* selectedRecord, float time, RecordPlayer& recordPlayer, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {
	MyImGui::ButtonWithIconDisabled(Textures::Play(), "No record selected");
	if (selectedRecord)
		recordPlayer.setState<PlayState_NotStarted>(*selectedRecord);
}