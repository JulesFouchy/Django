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
	App(SDL_Window* window);
	~App() = default;

private:
	void onLoopIteration();
	void onEvent(const SDL_Event& e);

	void onWindowResize();
	void onRenderTargetModified();
	void switchFullScreenMode();

private:
	bool m_bShowImGUIDemoWindow;
	bool m_bFullScreen;
	bool m_bShowGUI;

	ShaderPipeline m_particlePipeline;

	ParticleSystem m_particleSystem;
	SettingsManager m_settingsManager;
	ConfigManager m_configManager;
	Renderer m_renderer;
	RecordManager m_recordManager;
	MouseInteractions m_mouseInteractions;

	StateModifier m_stateModifier;

/********************
  * INTERNAL CODE *
 ********************/

public:
	void _loopIteration();
	inline bool _isRunning() const { return m_running; }

private:
	inline void exit() { m_running = false; }

private:
	SDL_Window* m_window;
	bool m_running;
};
