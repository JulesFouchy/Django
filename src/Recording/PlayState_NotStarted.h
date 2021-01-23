#pragma once

#include "PlayState.h"

class PlayState_NotStarted : public PlayState {
public:
	PlayState_NotStarted(Record& record, bool bStartJustAfterThat = true);
	~PlayState_NotStarted() = default;

	void ImGui (Record* selectedRecord, float time, RecordPlayer& recordPlayer, StateModifier& stateModifier) override;

private:
	Record& m_record;
	bool m_bStartJustAfterThat = true;
};