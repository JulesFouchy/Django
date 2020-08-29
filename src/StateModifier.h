#pragma once

class ParticleSystem;
class SettingsManager;
class ConfigManager;
class Renderer;
class RecordManager;

struct StateChange;
struct State;
struct Action;

class StateModifier {
public:
	StateModifier(ParticleSystem& particleSystem, SettingsManager& settingsManager, ConfigManager& configManager, Renderer& renderer, RecordManager& recordManager);
	~StateModifier() = default;

	void applyAndRecord(const StateChange& stateChange);
	void applyAndRecord(const State& state);
	void apply();
	void applyAllSettings();
	void recordChange(const StateChange& stateChange);

	void setNbParticles(unsigned int nbParticles);
	void setParticleRadius(float radiusRelToHeight);

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
};