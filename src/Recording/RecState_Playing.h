#pragma once

#include "RecState.h"

class Record;

class RecState_Playing : public RecState {
public:
	RecState_Playing(RecordManager& recManager, Record& record, ConfigManager& configManager, ParticlesSystem& partSystem);
	~RecState_Playing() = default;

	void update(ConfigManager& configManager, ParticlesSystem& partSystem) override;
	void ImGui (ConfigManager& configManager, ParticlesSystem& partSystem) override;

private:
	float timeSinceStart();

private:
	Record& m_record;
	float m_startTime;

	bool m_bDraggingOnTheTimeline = false;
};