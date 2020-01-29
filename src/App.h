#pragma once

struct SDL_Window;
union SDL_Event;

#include "Particles/ParticlesSystem.h"
#include "OpenGL/ShaderPipeline.h"

#include "Configurations/Config_FillScreen.h"

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

private:
	bool m_bShowImGUIDemoWindow;
	bool m_bFullScreen;

	ShaderPipeline m_pipeline;
	ParticlesSystem m_particlesSystem;
	Config_FillScreen m_configFillScreen;

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
