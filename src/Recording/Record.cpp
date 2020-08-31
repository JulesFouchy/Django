#include "Record.h"

#include "StateModifier.h"
#include "Helper/Time.h"
#include "Helper/String.h"
#include <cereal/archives/binary.hpp>
#include <fstream>

static constexpr float APPLY_BURST_TIME_THRESHOLD = 1.0f;

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
		// Apply everyting up to newTime
		while (m_nextStateChangeIdx < m_stateChangesTimeline.size() && nextStateChangeTS().time <= newTime) {
			setApplyRecord_WithChecks(nextStateChangeTS(), prevTime, newTime, stateModifier);
			m_nextStateChangeIdx++;
		}
		// Check for mouse bursts to apply if we are moving backward
		if (prevTime > newTime) {
			size_t stateChangeIdx = m_nextStateChangeIdx;
			while (stateChangeIdx < m_stateChangesTimeline.size() && m_stateChangesTimeline[stateChangeIdx].time <= newTime + APPLY_BURST_TIME_THRESHOLD) { // prospect forward 
				const StateChangeTimestamp& stateChangeTS = m_stateChangesTimeline[stateChangeIdx];
				if (stateChangeTS.stateChange.type == StateChangeType::Mouse_Burst 
					&& stateChangeTS.time < prevTime) // we didn't apply the burst last time
					stateModifier.setApplyAndRecord(stateChangeTS.stateChange);
				stateChangeIdx++;
			}
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

void Record::setApplyRecord_WithChecks(const StateChangeTimestamp& stateChangeTS, float prevTime, float newTime, StateModifier& stateModifier) {
	switch (stateChangeTS.stateChange.type)
	{
	case StateChangeType::Mouse_ForceField:
		if (abs(stateChangeTS.time - newTime) > 0.5f) // only apply force field if we set time close to it's actual timestamp
			return;
		break;
	case StateChangeType::Mouse_Burst: // only apply the mouse bursts under specific conditions because they cause problem when we apply them repeatedly (e.g. when dragging on the timeline, we call setTime() each frame)
		if (stateChangeTS.time < prevTime // we did apply the burst last time
			|| (newTime - stateChangeTS.time) > 1.0f) // we did jump too far in time from the burst
			return;
		break;
	}
	stateModifier.setApplyAndRecord(stateChangeTS.stateChange);
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