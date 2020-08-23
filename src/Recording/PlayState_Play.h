#pragma once

#include "PlayState.h"

class PlayState_Play : public PlayState {
public:
	PlayState_Play(Record& record, float startTime);
	~PlayState_Play() = default;

	void update(float time,                         RecordPlayer& recordPlayer, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) override;
	void ImGui (Record* selectedRecord, float time, RecordPlayer& recordPlayer, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) override;

private:
	Record& m_record;
	float m_startTime;
	bool m_bDraggingOnTheTimeline = false;
};