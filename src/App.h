#pragma once

#include "OpenGL/ShaderPipeline.h"
#include "Particles/ParticlesSystem.h"
#include "Settings/SettingsManager.h"
#include "Configurations/ConfigManager.h"
#include "Renderer.h"
#include "Recording/RecordManager.h"
#include "MouseInteractions.h"
#include "StateModifier.h"

class App {
public:
	App();
	~App() = default;

	void update();
	void onEvent(const SDL_Event& e);

	void onRenderAreaResize();

private:
	void onRenderTargetModified();

private:
	bool m_bShowImGUIDemoWindow = false;
	bool m_bShowGUI = true;

	ShaderPipeline m_particlePipeline;
	ParticleSystem m_particleSystem;
	SettingsManager m_settingsManager;
	ConfigManager m_configManager;
	Renderer m_renderer;
	RecordManager m_recordManager;
	MouseInteractions m_mouseInteractions;
	StateModifier m_stateModifier; // must be declared last because it relies on all the systems to be initialized
};
