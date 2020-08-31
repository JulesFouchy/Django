#pragma once

#include "State.h"
#include "StateChangeTimestamp.h"

class StateModifier;

class Record {
public:
	Record() = default;
	Record(const State& currentState);
	Record(const std::string& filepath);
	~Record() = default;
	void init(const State& currentState);

	void recordStateChange(const StateChange& stateChange, float timestamp);
	bool startPlaying(StateModifier& stateModifier); // Returns true iff we should keep playing the record
	bool updatePlaying(float time, StateModifier& stateModifier); // Returns true iff we should keep playing the record

	bool setTime(float newTime, StateModifier& stateModifier); // Returns true iff we should keep playing the record
	inline float initialTime() const { return m_startState.timestamp; }
	inline float totalDuration() const { return m_stateChangesTimeline.size() > 0 ? m_stateChangesTimeline.back().time : 0.0f; }
	inline const std::string& name() const { return m_name; }

	void serialize(const std::string& folderPath);

private:
	void deserialize(const std::string& filepath);

	const StateChangeTimestamp& nextStateChangeTS() const;
	void advanceOnTimeline(StateModifier& stateModifier);
	void setApplyRecord_WithChecks(const StateChangeTimestamp& stateChangeTS, float prevTime, float newTime, StateModifier& stateModifier);

private:
	State m_startState;
	std::vector<StateChangeTimestamp> m_stateChangesTimeline;
	size_t m_nextStateChangeIdx;
	std::string m_name;
	
	float m_prevTime = 0.0f;
};