#pragma once

class RecordManager;
class ConfigManager;
class ParticlesSystem;
struct ActionRef;

class RecState {
public:
	RecState::RecState(RecordManager& recManager)
		: R(recManager)
	{}
	virtual ~RecState() = default;

	virtual void update(ConfigManager& configManager, ParticlesSystem& partSystem) {};
	virtual void ImGui (ConfigManager& configManager, ParticlesSystem& partSystem) = 0;
	virtual void onAction(const ActionRef& actionRef)                              {};

protected:
	RecordManager& R;
};