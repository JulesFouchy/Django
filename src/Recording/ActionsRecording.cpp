#include "ActionsRecording.h"

void ActionsRecording::start(const ActionRef& startAction, float time) {
	m_startAction = startAction;
	m_startTime = time;
}

void ActionsRecording::push(const ActionRef& actionRef, float time) {
	m_timeline.emplace_back(actionRef, time);
}