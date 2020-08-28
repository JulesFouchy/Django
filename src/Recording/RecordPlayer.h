#pragma once

#include "PlayState.h"

class Record;
class StateModifier;

class RecordPlayer {
public:
	RecordPlayer();
	~RecordPlayer() = default;

	void update(float time, StateModifier& stateModifier);
	void ImGui (Record* selectedRecord, float time, StateModifier& stateModifier);

	template<typename T, typename... Args>
	void setState(Args&&... args) {
		m_playState = std::make_unique<T>(std::forward<Args>(args)...);
	}

private:
	std::unique_ptr<PlayState> m_playState;
	Record* m_record = nullptr;
	float m_startTime;

	bool m_bDraggingOnTheTimeline = false;
};