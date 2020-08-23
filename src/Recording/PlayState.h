#pragma once

class RecordPlayer;

class Record;
class ConfigManager;
class ParticlesSystem;
class RecordManager;
class Clock;

class PlayState {
public:
	PlayState() = default;
	virtual ~PlayState() = default;

	virtual void update(float time,                           RecordPlayer& recordPlayer, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) {};
	virtual void ImGui (Record* selectedRecord, Clock& clock, RecordPlayer& recordPlayer, ConfigManager& configManager, ParticlesSystem& partSystem, RecordManager& recordManager) = 0;
};