#include "Recording.h"

#include "Configurations/ConfigManager.h"

Recording::Recording(const ConfigRef& initialConfiguration)
	: m_startState(initialConfiguration)
{}

void Recording::onAction(const ActionRef& actionRef, float timestamp) {
	m_actionsTimeline.emplace_back(actionRef, timestamp);
}

bool Recording::startPlaying(ConfigManager& configManager, ParticlesSystem& partSystem) {
	configManager.applyConfigRef(m_startState.configRef);
	configManager.applyTo(partSystem);
	m_nextActionIdx = 0;
	return m_actionsTimeline.size() != 0;
}

bool Recording::updatePlaying(float time, ConfigManager& configManager, ParticlesSystem& partSystem) {
	while (m_actionsTimeline[m_nextActionIdx].time < time) {
		// Apply action
		configManager.applyActionRef(m_actionsTimeline[m_nextActionIdx].actionRef);
		configManager.applyTo(partSystem);
		// Move to next action
		m_nextActionIdx++;
		if (m_nextActionIdx >= m_actionsTimeline.size())
			return false;
	}
	return true;
}