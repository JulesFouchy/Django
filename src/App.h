#pragma once

struct SDL_Window;
union SDL_Event;

#include "Particles/ParticlesSystem.h"
#include "OpenGL/ShaderPipeline.h"
#include "OpenGL/QuadVAO.h"

#include "Configurations/Config_FillScreen.h"
#include "Configurations/Config_RandomCPU.h"
#include "Configurations/Config_RandomGPU.h"
#include "Configurations/Config_Circle.h"
#include "Configurations/Config_ClusterRandom.h"

#include "Time/Time_Realtime.h"

class App {
private:
	App(unsigned int nbParticles, SDL_Window* window);
	~App() = default;

public:
	void onInit();
private:
	void setCurrentConfiguration(Configuration& newConfig);

	void onLoopIteration();
	void onEvent(const SDL_Event& e);

	void onWindowResize();
	void switchFullScreenMode();

private:
	bool m_bShowImGUIDemoWindow;
	bool m_bFullScreen;

	ShaderPipeline m_particlePipeline;
	ShaderPipeline m_clearScreenPipeline;
	QuadVAO m_fullScreenVAO;
	Time_Realtime m_time;
	ParticlesSystem m_particlesSystem;

	Configuration* m_currentConfig;
	Config_FillScreen m_configFillScreen;
	Config_RandomCPU m_configRandomCPU;
	Config_RandomGPU m_configRandomGPU;
	Config_Circle m_configCircle;
	Config_ClusterRandom m_configClusterRandom;

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
