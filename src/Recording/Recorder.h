#pragma once

#include "ActionsRecording.h"
#include "Time/Time.h"

class Recorder {
public:
	Recorder();
	~Recorder() = default;

	void onAction(const ActionRef& actionRef);

	inline const Time& time() const { return *m_time; }
	void update();

private:
	std::unique_ptr<Time> m_time;
	bool m_bRecording = false;
	ActionsRecording m_actionsRecording;
};