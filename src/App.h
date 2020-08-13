#pragma once

#include "Particles/ParticlesSystem.h"
#include "OpenGL/ShaderPipeline.h"
#include "Configurations/ConfigManager.h"
#include "Settings/SettingsManager.h"
#include "Recording/Recorder.h"

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
	void switchFullScreenMode();

	void beforeShutDown();

private:
	bool m_bShowImGUIDemoWindow;
	bool m_bFullScreen;
	bool m_bShowGUI;

	ShaderPipeline m_particlePipeline;
	ParticlesSystem m_particlesSystem;

	ConfigManager m_configManager;
	SettingsManager m_settingsMng;

	Recorder m_recorder;

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
