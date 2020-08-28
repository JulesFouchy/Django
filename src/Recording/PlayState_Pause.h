#pragma once

#include "PlayState.h"

class PlayState_Pause : public PlayState {
public:
	PlayState_Pause(Record& record, float timeSinceStart);
	~PlayState_Pause() = default;

	void ImGui (Record* selectedRecord, float time, RecordPlayer& recordPlayer, StateModifier& stateModifier) override;

private:
	Record& m_record;
	float m_timeSinceStart;
};