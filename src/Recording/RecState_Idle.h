#pragma once

#include "RecState.h"

class RecState_Idle : public RecState {
public:
	RecState_Idle(RecordManager& recManager)
		: RecState(recManager)
	{}
	void update(ConfigManager& configManager, ParticlesSystem& partSystem) override;
	void ImGui (ConfigManager& configManager, ParticlesSystem& partSystem) override;
};