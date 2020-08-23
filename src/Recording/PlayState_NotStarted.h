#pragma once

#include "PlayState.h"

class PlayState_NotStarted : public PlayState {
public:
	PlayState_NotStarted(Record& record);
	~PlayState_NotStarted() = default;

	void ImGui (Record* selectedRecord, float time, RecordPlayer& recordPlayer, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) override;

private:
	Record& m_record;
};