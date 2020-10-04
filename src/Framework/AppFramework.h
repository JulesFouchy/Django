#pragma once

#include "App.h"
#include "GLWindow.h"

struct ImGuiDockNode;

class AppFramework {
public:
	AppFramework(GLWindow& glWindow, App& app);
	~AppFramework() = default;

	void update();
	inline bool isRunning() const { return m_running; }

private:
	void onWindowMove();
	void onWindowResize();
	void updateRenderArea(ImGuiDockNode* node);
	void onEvent(const SDL_Event& e);
	void ImGuiDockspace();
	inline void exit() { m_running = false; }

private:
	GLWindow& m_glWindow;
	App& m_app;
	bool m_running = true;
};
