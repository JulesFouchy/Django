#include "Record.h"

#include "StateModifier.h"
#include "Helper/Time.h"
#include "Helper/String.h"
#include <cereal/archives/binary.hpp>
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
	m_prevTime = 0.0f;
	stateModifier.setApplyAndRecord(m_startState);
	m_nextStateChangeIdx = 0;
	return m_stateChangesTimeline.size() != 0;
}

bool Record::updatePlaying(float time, StateModifier& stateModifier) {
	m_prevTime = time;
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
	float prevTime = m_prevTime;
	m_prevTime = newTime;
	stateModifier.setApplyAndRecord(m_startState);
	if (m_stateChangesTimeline.size() == 0)
		return false;
	else {
		m_nextStateChangeIdx = 0;
		while (m_nextStateChangeIdx < m_stateChangesTimeline.size() && nextStateChangeTS().time <= newTime) {
			setApplyRecord_WithChecks(stateModifier, prevTime, newTime);
			m_nextStateChangeIdx++;
		}
		return m_nextStateChangeIdx < m_stateChangesTimeline.size();
	}
}

const StateChangeTimestamp& Record::nextStateChangeTS() const {
	return m_stateChangesTimeline[m_nextStateChangeIdx];
}

void Record::advanceOnTimeline(StateModifier& stateModifier) {
	stateModifier.setApplyAndRecord(nextStateChangeTS().stateChange);
	m_nextStateChangeIdx++;
}

void Record::setApplyRecord_WithChecks(StateModifier& stateModifier, float prevTime, float newTime) {
	switch (nextStateChangeTS().stateChange.type)
	{
	case StateChangeType::Mouse_ForceField:
		if (abs(nextStateChangeTS().time - newTime) > 0.5f) // only apply force field if we set time close to it's actual timestamp
			return;
		break;
	case StateChangeType::Mouse_Burst: // only apply the mouse bursts under specific conditions because they cause problem when we apply them repeatedly (e.g. when dragging on the timeline, we call setTime() each frame)
		if (nextStateChangeTS().time < prevTime // we did apply the burst last time
			|| (newTime - nextStateChangeTS().time) > 1.0f) // we did jump too far in time from the burst
			return;
		break;
	}
	stateModifier.setApplyAndRecord(nextStateChangeTS().stateChange);		
}

void Record::serialize(const std::string& folderPath) {
	std::ofstream os(folderPath + "/" + m_name + ".djgRecord", std::ios::binary);
	{
		cereal::BinaryOutputArchive archive(os);
		archive(
			m_startState,
			m_stateChangesTimeline
		);
	}
}

void Record::deserialize(const std::string& filepath) {
	std::ifstream is(filepath, std::ios::binary);
	{
		cereal::BinaryInputArchive archive(is);
		archive(
			m_startState,
			m_stateChangesTimeline
		);
	}
	m_name = MyString::FileName(filepath);
}