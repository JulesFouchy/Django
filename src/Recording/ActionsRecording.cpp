#include "ActionsRecording.h"

void ActionsRecording::start(float time, const ActionRef& startAction) {
	m_startAction = startAction;
	m_startTime = time;
}

void ActionsRecording::push(float time, const ActionRef& actionRef) {
	m_timeline.emplace_back(actionRef, time);
}