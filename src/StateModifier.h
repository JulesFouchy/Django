#pragma once

class ParticleSystem;
class SettingsManager;
class ConfigManager;
class Renderer;
class RecordManager;
class MouseInteractions;

struct StateChange;
#include "Recording/State.h"
struct Action;

class StateModifier {
public:
	StateModifier(ParticleSystem& particleSystem, SettingsManager& settingsManager, ConfigManager& configManager, Renderer& renderer, RecordManager& recordManager, MouseInteractions& mouseInteractions);
	~StateModifier() = default;

	void setAndRecord(const StateChange& stateChange);
	void setApplyAndRecord(const StateChange& stateChange);
	void setApplyAndRecord(const State& state, bool bSetInitialColorAndParticles = false);
	void apply();
	void applyAndRecord_CurrentAction();
	void applyAndRecord_AllSettings();
	void recordChange(const StateChange& stateChange);

	State getCurrentState() const;

	ParticleSystem&  particleSystem()  { return m_particleSystem; }
	SettingsManager& settingsManager() { return m_settingsManager; }
	ConfigManager&   configManager()   { return m_configManager; }
	Renderer&        renderer()        { return m_renderer; }
	RecordManager&   recordManager()   { return m_recordManager; }

private:
	ParticleSystem& m_particleSystem;
	SettingsManager& m_settingsManager;
	ConfigManager& m_configManager;
	Renderer& m_renderer;
	RecordManager& m_recordManager;
	MouseInteractions& m_mouseInteractions;

	bool m_bBlockApplying = false; // Ugly thing "required" by setAndRecord(StateChange)
};