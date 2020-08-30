#pragma once

#include "Record.h"

struct ConfigRef;
class StateModifier;

class Recorder {
public:
	Recorder() = default;
	~Recorder();

	void recordChange(const StateChange& stateChange, float time);
	bool ImGui(float time, const StateModifier& stateModifier);

	inline const Record& getRecord() { return m_record; }

private:
	void start(const State& currentState);
	void stop();

	inline bool isRecording() { return m_bIsRecording; }

private:
	float m_startTime;
	Record m_record;
	bool m_bIsRecording = false;
};