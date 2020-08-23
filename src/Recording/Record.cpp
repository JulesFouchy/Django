#include "Record.h"

#include "Configurations/ConfigManager.h"
#include "Helper/Time.h"
#include <cereal/archives/json.hpp>
#include <fstream>

Record::Record(const ConfigRef& initialConfiguration) {
	init(initialConfiguration);
}

Record::Record(const std::string& filepath) {
	deserialize(filepath);
}

void Record::init(const ConfigRef& initialConfiguration) {
	m_startState = State(initialConfiguration);
	m_name = MyTime::AsString();
	m_actionsTimeline.clear();
}

void Record::onAction(const ActionRef& actionRef, float timestamp) {
	m_actionsTimeline.emplace_back(actionRef, timestamp);
}

bool Record::startPlaying(ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {
	applyConfig(m_startState.configRef, configManager, partSystem, recordManager);
	m_nextActionIdx = 0;
	return m_actionsTimeline.size() != 0;
}

bool Record::updatePlaying(float time, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {
	if (m_nextActionIdx < m_actionsTimeline.size()) {
		while (m_actionsTimeline[m_nextActionIdx].time < time) {
			// Apply action
			applyAction(m_nextActionIdx, configManager, partSystem, recordManager);
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

bool Record::setTime(float newTime, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {
	// No actions
	if (m_actionsTimeline.size() == 0)
		return false;
	// Time is bigger than duration
	else if (m_actionsTimeline.back().time < newTime) {
		// Apply last action
		applyAction(m_actionsTimeline.size() - 1, configManager, partSystem, recordManager);
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
			applyAction(i - 1, configManager, partSystem, recordManager);
		}
		else {
			applyConfig(m_startState.configRef, configManager, partSystem, recordManager);
		}
		m_nextActionIdx = i;
	}
}

void Record::applyConfig(const ConfigRef& configRef, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {
	configManager.applyConfigRef(configRef, recordManager);
	configManager.applyTo(partSystem);
}

void Record::applyAction(size_t idx, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {
	configManager.applyActionRef(m_actionsTimeline[idx].actionRef, recordManager);
	configManager.applyTo(partSystem);
}

void Record::serialize(const std::string& folderPath) {
	std::ofstream os(folderPath + "/" + m_name + ".json");
	{
		cereal::JSONOutputArchive archive(os);
		archive(
			CEREAL_NVP(m_startState),
			CEREAL_NVP(m_actionsTimeline)
		);
	}
}

void Record::deserialize(const std::string& filepath) {
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