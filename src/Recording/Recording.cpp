#include "Recording.h"

#include "Configurations/ConfigManager.h"
#include "Helper/Time.h"
#include <cereal/archives/json.hpp>
#include <fstream>

Recording::Recording(const ConfigRef& initialConfiguration)
	: m_startState(initialConfiguration), m_name(MyTime::AsString())
{}

Recording::Recording(const std::string& filepath) {
	deserialize(filepath);
}

void Recording::onAction(const ActionRef& actionRef, float timestamp) {
	m_actionsTimeline.emplace_back(actionRef, timestamp);
}

bool Recording::startPlaying(ConfigManager& configManager, ParticlesSystem& partSystem) {
	applyConfig(m_startState.configRef, configManager, partSystem);
	m_nextActionIdx = 0;
	return m_actionsTimeline.size() != 0;
}

bool Recording::updatePlaying(float time, ConfigManager& configManager, ParticlesSystem& partSystem) {
	if (m_nextActionIdx < m_actionsTimeline.size()) {
		while (m_actionsTimeline[m_nextActionIdx].time < time) {
			// Apply action
			applyAction(m_nextActionIdx, configManager, partSystem);
			// Move to next action
			m_nextActionIdx++;
			if (m_nextActionIdx >= m_actionsTimeline.size())
				return false;
		}
		return true;
	}
	else {
		return false;
	}
}

bool Recording::setTime(float newTime, ConfigManager& configManager, ParticlesSystem& partSystem) {
	// No actions
	if (m_actionsTimeline.size() == 0)
		return false;
	// Time is bigger than duration
	else if (m_actionsTimeline.back().time < newTime) {
		// Apply last action
		applyAction(m_actionsTimeline.size() - 1, configManager, partSystem);
		//
		return false;
	}
	// There is still some duration left
	else {
		// Find
		size_t i = 0;
		while (m_actionsTimeline[i].time < newTime) { // TODO could use dichotomic search since the vector is sorted
			i++;
		}
		// Apply state
		if (i != 0) {
			applyAction(i - 1, configManager, partSystem);
		}
		else {
			applyConfig(m_startState.configRef, configManager, partSystem);
		}
		m_nextActionIdx = i;
	}
}

void Recording::applyConfig(const ConfigRef& configRef, ConfigManager& configManager, ParticlesSystem& partSystem) {
	configManager.applyConfigRef(configRef);
	configManager.applyTo(partSystem);
}

void Recording::applyAction(size_t idx, ConfigManager& configManager, ParticlesSystem& partSystem) {
	configManager.applyActionRef(m_actionsTimeline[idx].actionRef);
	configManager.applyTo(partSystem);
}

void Recording::serialize(const std::string& folderPath) {
	std::ofstream os(folderPath + "/" + m_name + ".json");
	{
		cereal::JSONOutputArchive archive(os);
		archive(
			CEREAL_NVP(m_startState),
			CEREAL_NVP(m_actionsTimeline)
		);
	}
}

void Recording::deserialize(const std::string& filepath) {
	std::ifstream is(filepath);
	{
		cereal::JSONInputArchive archive(is);
		archive(
			CEREAL_NVP(m_startState),
			CEREAL_NVP(m_actionsTimeline)
		);
	}
	m_name = MyString::FileName(filepath);
}