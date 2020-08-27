#pragma once

#include "State.h"
#include "EventTimestamp.h"

class ConfigManager;
class ParticlesSystem;
class RecordManager;

class Record {
public:
	Record() = default;
	Record(float timestamp, const ConfigRef& initialConfiguration);
	Record(const std::string& filepath);
	~Record() = default;
	void init(float timestamp, const ConfigRef& initialConfiguration);

	void recordEvent(const Event& event, float timestamp);
	bool startPlaying(             ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager); // Returns true iff we should keep playing the record
	bool updatePlaying(float time, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager); // Returns true iff we should keep playing the record

	bool setTime(float newTime,    ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager); // Returns true iff we should keep playing the record
	inline float initialTime() const { return m_startState.timestamp; }
	inline float totalDuration() const { return m_eventsTimeline.size() > 0 ? m_eventsTimeline.back().time : 0.0f; }
	inline const std::string& name() const { return m_name; }

	void serialize(const std::string& folderPath);

private:
	void applyConfig(const ConfigRef& configRef, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager);
	void applyAction(size_t idx,                 ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager);

	void deserialize(const std::string& filepath);

private:
	State m_startState;
	std::vector<EventTimestamp> m_eventsTimeline;
	size_t m_nextActionIdx;
	std::string m_name;
};