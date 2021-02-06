#pragma once

#include <Cool/App/IApp.h>
#include <Cool/App/OpenGLWindow.h>

#include "OpenGL/ShaderPipeline.h"
#include "Particles/ParticlesSystem.h"
#include "Settings/SettingsManager.h"
#include "Configurations/ConfigManager.h"
#include "Renderer.h"
#include "Recording/RecordManager.h"
#include "MouseInteractions.h"
#include "StateModifier.h"

using namespace Cool;

class App : public Cool::IApp {
public:
	App(OpenGLWindow& mainWindow, OpenGLWindow& outputWindow);
	~App() = default;

	void update();
	void onEvent(const SDL_Event& e);

	void onRenderSizeChanged();

private:
	void setIsOutputWindowOpen(bool isOpen);

private:
	bool m_bShowImGUIDemoWindow = false;
	bool m_bShowGUI = true;
	glm::vec3 m_clearColor = glm::vec3(0.5f);

	ShaderPipeline m_particlePipeline;
	ParticleSystem m_particleSystem;
	SettingsManager m_settingsManager;
	ConfigManager m_configManager;
	Renderer m_renderer;
	RecordManager m_recordManager;
	MouseInteractions m_mouseInteractions;
	StateModifier m_stateModifier; // must be declared last because it relies on all the systems to be initialized

	bool m_bOpenKeyBindings = true;
	bool m_bOpenRecordings = true;
#ifndef NDEBUG
	bool m_bOpenDebug = true;
#endif

	OpenGLWindow& m_mainWindow;
	OpenGLWindow& m_outputWindow;
};
