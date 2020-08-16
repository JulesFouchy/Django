#include "Recording.h"

Recording::Recording(const ConfigRef& initialConfiguration)
	: m_startState(initialConfiguration)
{}

void Recording::onAction(const ActionRef& actionRef, float timestamp) {
	m_actionsTimeline.emplace_back(actionRef, timestamp);
}