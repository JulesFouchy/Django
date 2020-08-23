#pragma once

#include "RecState.h"

struct ConfigRef;
class Record;

class RecState_Recording : public RecState {
public:
	RecState_Recording(RecordManager& recManager, const ConfigRef& currentConfigRef);
	~RecState_Recording();

	void update(ConfigManager& configManager, ParticlesSystem& partSystem) override;
	void ImGui (ConfigManager& configManager, ParticlesSystem& partSystem) override;
	void onAction(const ActionRef& actionRef) override;

private:
	float timeSinceStart();

private:
	float m_startTime;
	Record& m_record;
};