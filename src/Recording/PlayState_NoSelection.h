#pragma once

#include "PlayState.h"

class PlayState_NoSelection : public PlayState {
public:
	PlayState_NoSelection() = default;
	~PlayState_NoSelection() = default;

	void ImGui(Record* selectedRecord, Clock& clock, RecordPlayer& recordPlayer, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) override;
};