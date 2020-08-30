#pragma once

#include "Record.h"

struct ConfigRef;

class Recorder {
public:
	Recorder() = default;
	~Recorder();

	void recordChange(const StateChange& stateChange, float time);
	bool ImGui(float time);

	inline const Record& getRecord() { return m_record; }

private:
	void start(float time);
	void stop();

	inline bool isRecording() { return m_bIsRecording; }

private:
	float m_startTime;
	Record m_record;
	bool m_bIsRecording = false;
};