#pragma once

class RecordPlayer;
class Record;
class StateModifier;

class PlayState {
public:
	PlayState() = default;
	virtual ~PlayState() = default;

	virtual void update(float time, RecordPlayer& recordPlayer, StateModifier& stateModifier) {};
	virtual void ImGui (Record* selectedRecord, float time, RecordPlayer& recordPlayer, StateModifier& stateModifier) = 0;
};