#pragma once

#include "Record.h"

struct ConfigRef;
class StateModifier;
class Clock;

class Recorder {
public:
	Recorder(Clock& clock);
	~Recorder();

	void recordChange(const StateChange& stateChange, float time);
	bool ImGui(float time, const StateModifier& stateModifier);

	inline const Record& getRecord() { return m_record; }

private:
	void start(const State& currentState);
	void stop(float currentTime);
	void startCountdownBeforeRecording();

	inline bool isRecording() { return m_bIsRecording; }

private:
	float m_startTime;
	Record m_record;
	bool m_bIsRecording = false;
	int m_delayToStartRecordingInS = 3;
	bool m_bIsCountingDown = false;
	std::chrono::steady_clock::time_point m_timeToStart;

	Clock& m_clock; // we only need it to grab time in the destructor, because we can't pass it as an argument there
};