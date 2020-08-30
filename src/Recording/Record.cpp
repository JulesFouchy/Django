#include "Record.h"

#include "StateModifier.h"
#include "Helper/Time.h"
#include "Helper/String.h"
#include <cereal/archives/json.hpp>
#include <fstream>

Record::Record(const State& currentState) {
	init(currentState);
}

Record::Record(const std::string& filepath) {
	deserialize(filepath);
}

void Record::init(const State& currentState) {
	m_startState = currentState;
	m_name = MyTime::AsString();
	m_stateChangesTimeline.clear();
}

void Record::recordStateChange(const StateChange& stateChange, float timestamp) {
	m_stateChangesTimeline.emplace_back(stateChange, timestamp);
}

bool Record::startPlaying(StateModifier& stateModifier) {
	stateModifier.applyAndRecord(m_startState);
	m_nextStateChangeIdx = 0;
	return m_stateChangesTimeline.size() != 0;
}

bool Record::updatePlaying(float time, StateModifier& stateModifier) {
	if (m_nextStateChangeIdx < m_stateChangesTimeline.size()) {
		while (nextStateChangeTS().time < time) {
			advanceOnTimeline(stateModifier);
			if (m_nextStateChangeIdx >= m_stateChangesTimeline.size())
				return false;
		}
		return true;
	}
	else {
		return false;
	}
}

bool Record::setTime(float newTime, StateModifier& stateModifier) {
	// No actions
	if (m_stateChangesTimeline.size() == 0)
		return false;
	// Time is bigger than duration
	//else if (m_stateChangesTimeline.back().time <= newTime) {
	//	// Apply last action
	//	stateModifier.applyAndRecord(m_stateChangesTimeline.back().stateChange);
	//	//
	//	return false;
	//}
	// There is still some duration left
	else {
		stateModifier.applyAndRecord(m_startState);
		m_nextStateChangeIdx = 0;
		while (nextStateChangeTS().time < newTime) {
			advanceOnTimeline(stateModifier, false);
		}
		return true;
	}
}

const StateChangeTimestamp& Record::nextStateChangeTS() const {
	return m_stateChangesTimeline[m_nextStateChangeIdx];
}

void Record::advanceOnTimeline(StateModifier& stateModifier, bool bPlayTheMouseBursts) {
	if (bPlayTheMouseBursts || nextStateChangeTS().stateChange.type != StateChangeType::Mouse_Burst) // skip te mouse bursts because they cause problem when we setTime() repeatedly (e.g. when dragging on the timeline)
		stateModifier.applyAndRecord(nextStateChangeTS().stateChange);
	m_nextStateChangeIdx++;
}

void Record::serialize(const std::string& folderPath) {
	std::ofstream os(folderPath + "/" + m_name + ".json");
	{
		cereal::JSONOutputArchive archive(os);
		archive(
			CEREAL_NVP(m_startState),
			CEREAL_NVP(m_stateChangesTimeline)
		);
	}
}

void Record::deserialize(const std::string& filepath) {
	std::ifstream is(filepath);
	{
		cereal::JSONInputArchive archive(is);
		archive(
			CEREAL_NVP(m_startState),
			CEREAL_NVP(m_stateChangesTimeline)
		);
	}
	m_name = MyString::FileName(filepath);
}