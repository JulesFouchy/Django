#pragma once

#include "App.h"

class AppFramework {
public:
	AppFramework(SDL_Window* window, App& app);
	~AppFramework() = default;

	void update();
	inline bool isRunning() const { return m_running; }

private:
	void switchFullScreenMode();
	inline void exit() { m_running = false; }
	void onEvent(SDL_Event e);
	void dockspace();
	void onWindowMove();
	void onWindowResize();

private:
	SDL_Window* m_window;
	App& m_app;
	bool m_running = true;
	bool m_bFullScreen = false;
};
