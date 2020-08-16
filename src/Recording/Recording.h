#pragma once

#include "State.h"
#include "ActionTimestamp.h"

class Recording {
public:
	Recording(const ConfigRef& initialConfiguration);
	~Recording() = default;

	void onAction(const ActionRef& actionRef, float timestamp);

private:
	State m_startState;
	std::vector<ActionTimestamp> m_actionsTimeline;
};