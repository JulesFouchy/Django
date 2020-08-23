#pragma once

#include "PlayState.h"

class Record;
class ConfigManager;
class ParticlesSystem;
class RecordManager;
class Clock;

class RecordPlayer {
public:
	RecordPlayer();
	~RecordPlayer() = default;

	void update(float time,                           ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager);
	void ImGui (Record* selectedRecord, Clock& clock, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager);

	template<typename T, typename... Args>
	void setState(Args&&... args) {
		m_playState = std::make_unique<T>(std::forward<Args>(args)...);
	}

private:
	std::unique_ptr<PlayState> m_playState;
	Record* m_record = nullptr;
	float m_startTime;

	bool m_bDraggingOnTheTimeline = false;
};