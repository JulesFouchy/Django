#pragma once

class Record;
class ConfigManager;
class ParticlesSystem;
class RecordManager;
class Clock;

class RecordPlayer {
public:
	RecordPlayer() = default;
	~RecordPlayer() = default;

	void update(float time,                           ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager);
	void ImGui (Record* selectedRecord, Clock& clock, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager);

private:
	void start(Record* record, float time,            ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager);
	void stop();
	inline bool isPlaying() { return (bool)m_record; }

private:
	Record* m_record = nullptr;
	float m_startTime;

	bool m_bDraggingOnTheTimeline = false;
};