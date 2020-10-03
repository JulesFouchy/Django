#pragma once

#include "App.h"

class AppFramework {
public:
	AppFramework(SDL_Window* window, App& app);
	~AppFramework() = default;

	void update();
	inline bool isRunning() const { return m_running; }

private:
	void onWindowMove();
	void onWindowResize();
	void switchFullScreenMode();
	void onEvent(SDL_Event e);
	void ImGuiDockspace();
	inline void exit() { m_running = false; }

private:
	SDL_Window* m_window;
	App& m_app;
	bool m_running = true;
	bool m_bFullScreen = false;
};
