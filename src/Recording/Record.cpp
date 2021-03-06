#include "Record.h"

#include "StateModifier.h"
#include "Renderer.h"
#include "Settings/SettingsManager.h"
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

bool Record::startPlaying(StateModifier& stateModifier, bool bSetInitialColorAndParticles) {
	m_prevTime = 0.0f;
	stateModifier.setApplyAndRecord(m_startState, bSetInitialColorAndParticles);
	if (bSetInitialColorAndParticles)
		stateModifier.renderer().clearRenderBuffer(stateModifier.settingsManager().get().colors().backgroundColor());
	m_nextStateChangeIdx = 0;
	return m_stateChangesTimeline.size() != 0;
}

bool Record::updatePlaying(float time, StateModifier& stateModifier) {
	m_prevTime = time;
	while (m_nextStateChangeIdx < m_stateChangesTimeline.size() && nextStateChangeTS().time < time) {
		advanceOnTimeline(stateModifier);
	}
	return time < totalDuration();
}

bool Record::setTime(float newTime, StateModifier& stateModifier) {
	float prevTime = m_prevTime;
	m_prevTime = newTime;
	stateModifier.setApplyAndRecord(m_startState);
	if (m_stateChangesTimeline.size() != 0) {
		m_nextStateChangeIdx = 0;
		// Set everything up to newTime
		while (m_nextStateChangeIdx < m_stateChangesTimeline.size() && nextStateChangeTS().time <= newTime) {
			setAndRecord_WithChecks(nextStateChangeTS(), prevTime, newTime, stateModifier);
			m_nextStateChangeIdx++;
		}
		// Check for mouse bursts to apply if we are moving backward
		if (prevTime > newTime) {
			size_t stateChangeIdx = m_nextStateChangeIdx;
			while (stateChangeIdx < m_stateChangesTimeline.size() && m_stateChangesTimeline[stateChangeIdx].time <= newTime + APPLY_BURST_TIME_THRESHOLD) { // prospect forward 
				const StateChangeTimestamp& stateChangeTS = m_stateChangesTimeline[stateChangeIdx];
				if (stateChangeTS.stateChange.type == StateChangeType::Mouse_Burst 
					&& stateChangeTS.time < prevTime) // we didn't apply the burst last time
					stateModifier.setAndRecord(stateChangeTS.stateChange);
				stateChangeIdx++;
			}
		}
		stateModifier.apply(); // Apply only once at the end
	}
	return newTime < totalDuration();
}

const StateChangeTimestamp& Record::nextStateChangeTS() const {
	return m_stateChangesTimeline[m_nextStateChangeIdx];
}

void Record::advanceOnTimeline(StateModifier& stateModifier) {
	stateModifier.setApplyAndRecord(nextStateChangeTS().stateChange);
	m_nextStateChangeIdx++;
}

void Record::setAndRecord_WithChecks(const StateChangeTimestamp& stateChangeTS, float prevTime, float newTime, StateModifier& stateModifier) {
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
	stateModifier.setAndRecord(stateChangeTS.stateChange);
}

void Record::serialize(const std::string& folderPath) {
	std::string filePath = folderPath + "/" + m_name;
	while (MyFile::Exists(filePath + ".djgRecord")) {
		filePath += "_";
		m_name   += "_";
	}
	std::ofstream os(filePath + ".djgRecord", std::ios::binary);
	{
		cereal::BinaryOutputArchive archive(os);
		archive(
			m_startState,
			m_totalDuration,
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
			m_totalDuration,
			m_stateChangesTimeline
		);
	}
	m_name = MyString::FileName(filepath);
}

void Record::setName(const std::string& newName) {
	std::filesystem::rename(
		Path::Records + "/" + m_name  + ".djgRecord",
		Path::Records + "/" + newName + ".djgRecord"
	);
	m_name = newName;
}