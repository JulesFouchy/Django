#pragma once

#include "OpenGL/ShaderPipeline.h"

#include "Particles/ParticlesSystem.h"
#include "Settings/SettingsManager.h"
#include "Configurations/ConfigManager.h"
#include "Renderer.h"
#include "Recording/RecordManager.h"

#include "StateModifier.h"

class App {
private:
	App(SDL_Window* window);
	~App() = default;

public:
	void onInit();
private:
	void onLoopIteration();
	void onEvent(const SDL_Event& e);

	void onWindowResize();
	void onRenderTargetModified();
	void switchFullScreenMode();

	void beforeShutDown();

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

	StateModifier m_stateModifier;

/********************
  * INTERNAL CODE *
 ********************/

public:
	static void Initialize(SDL_Window* window);
	static void ShutDown();
	inline static App& Get() { return *m_instance; }

	void _loopIteration();
	void handleSDLevents();

	inline SDL_Window* getWindow() const { return m_window; }
	inline bool isRunning() const { return m_running; }
	inline void exit() { m_running = false; }

private:
	static App* m_instance;

	SDL_Window* m_window;
	bool m_running;
};
