#pragma once

#include "Record.h"

struct ActionRef;
struct ConfigRef;

class Recorder {
public:
	Recorder() = default;
	~Recorder();

	void onEvent(const Event& event, float time);
	bool ImGui(const ConfigRef& currentConfigRef, float time);

	inline const Record& getRecord() { return m_record; }

private:
	void start(const ConfigRef& currentConfigRef, float time);
	void stop();

	inline bool isRecording() { return m_bIsRecording; }

private:
	float m_startTime;
	Record m_record;
	bool m_bIsRecording = false;
};