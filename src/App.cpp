#include "App.h"

#include <SDL2/SDL.h>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include "Debugging/Log.h"
#include "Debugging/glException.h"

#include "Settings/VisualSettings.h"

#include "Helper/DisplayInfos.h"


App::App(unsigned int nbParticles, SDL_Window* window)
	: m_bShowImGUIDemoWindow(false),
	  m_bFullScreen(false),
	  m_particlesSystem(nbParticles),
	  m_window(window), m_running(true)
{
	// Create graphics pipeline for particles
	m_particlePipeline.addShader(ShaderType::Vertex,   "res/shaders/particle.vert");
	m_particlePipeline.addShader(ShaderType::Fragment, "res/shaders/particle.frag");
	m_particlePipeline.createProgram();
	// Clear screen pipeline
	m_clearScreenPipeline.addShader(ShaderType::Vertex,   "res/shaders/clearScreen.vert");
	m_clearScreenPipeline.addShader(ShaderType::Fragment, "res/shaders/clearScreen.frag");
	m_clearScreenPipeline.createProgram();
	//
	setCurrentConfiguration(m_configRandomGPU);
	//
	onWindowResize();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void App::onInit() {
}

void App::onLoopIteration() {
	// ImGui windows
#ifndef NDEBUG 
	ImGui::Begin("Debug");
	ImGui::Checkbox("Show Demo Window", &m_bShowImGUIDemoWindow);
	ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);
	ImGui::End();
	if (m_bShowImGUIDemoWindow) // Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		ImGui::ShowDemoWindow(&m_bShowImGUIDemoWindow);
#endif
	// Time
	ImGui::Begin("Time");
	ImGui::Text(("time : " + std::to_string(m_time.time())).c_str());
	ImGui::Text(("FPS  : " + std::to_string(1.0f/m_time.deltaTime())).c_str());
	ImGui::End();
	// Settings
	VisualSettings::ImGuiWindow();
	// Current configuration
	ImGui::Begin(m_currentConfig->getName().c_str());
	m_currentConfig->ImGuiParameters(m_particlesSystem);
	ImGui::End();
	// Send time to physics compute shader
	m_time.update();
	m_particlesSystem.physicsComputeShader().bind();
	m_particlesSystem.physicsComputeShader().setUniform1f("dt", m_time.deltaTime());
	m_particlesSystem.physicsComputeShader().unbind();
	// Clear screen
	if (VisualSettings::IsAlphaTrailEnabled()) {
		m_clearScreenPipeline.bind();
		m_clearScreenPipeline.setUniform1f("dt", m_time.deltaTime());
		m_clearScreenPipeline.setUniform1f("decay", VisualSettings::AlphaTrailDecay());
		m_clearScreenPipeline.setUniform3f("backgroundColor", VisualSettings::BackgroundColor()[0], VisualSettings::BackgroundColor()[1], VisualSettings::BackgroundColor()[2]);
		m_fullScreenVAO.bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	}
	else {
		glClearColor(VisualSettings::BackgroundColor()[0], VisualSettings::BackgroundColor()[1], VisualSettings::BackgroundColor()[2], 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	
	// Draw particles
	m_particlePipeline.bind();
	m_particlesSystem.draw();
	// Update particles physics
	m_particlesSystem.updatePositions();
	// ImGui
	m_particlesSystem.ImGui_Windows(*m_currentConfig);
}


void App::setCurrentConfiguration(Configuration& newConfig) {
	m_currentConfig = &newConfig;
	m_currentConfig->setup(m_particlesSystem.getNbParticles());
	m_currentConfig->applyTo(m_particlesSystem);
}

void App::onEvent(const SDL_Event& e) {
	switch (e.type) {

	case SDL_MOUSEMOTION:
		if (!ImGui::GetIO().WantCaptureMouse) {

		}
		break;

	case SDL_MOUSEWHEEL:
		if (!ImGui::GetIO().WantCaptureMouse){

		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (!ImGui::GetIO().WantCaptureMouse) {
			if (e.button.button == SDL_BUTTON_LEFT) {

			}
			else if (e.button.button == SDL_BUTTON_RIGHT) {

			}
			else if (e.button.button == SDL_BUTTON_MIDDLE) {

			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (!ImGui::GetIO().WantCaptureMouse) {

		}
		break;


	case SDL_KEYDOWN:
		if (e.key.keysym.scancode == SDL_SCANCODE_F11)
			switchFullScreenMode();
		if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE && m_bFullScreen)
			switchFullScreenMode();
		if (!ImGui::GetIO().WantTextInput) {
			if (e.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				if (m_currentConfig->reroll()) {
					m_currentConfig->applyTo(m_particlesSystem);
				}
			}
			else if (e.key.keysym.sym == 'a')
				setCurrentConfiguration(m_configRandomGPU);
			else if (e.key.keysym.sym == 'q')
				setCurrentConfiguration(m_configClusterRandom);
			else if (e.key.keysym.sym == 'z')
				setCurrentConfiguration(m_configFillScreen);
			else if (e.key.keysym.sym == 'e')
				setCurrentConfiguration(m_configCircle);
			else
				m_currentConfig->onKeyPressed(e.key.keysym.scancode, m_particlesSystem);
		}
		break;

	case SDL_KEYUP:
		if (!ImGui::GetIO().WantTextInput) {

		}
		break;

	case SDL_WINDOWEVENT:
		switch (e.window.event) {
		case SDL_WINDOWEVENT_RESIZED:
			onWindowResize();
			break;
		}
		break;

	case SDL_QUIT:
		exit();
		break;

	default:
		break;
	}
}

void App::onWindowResize() {
	DisplayInfos::RefreshSize(m_window);
	glViewport(0, 0, DisplayInfos::Width(), DisplayInfos::Height());
	m_particlesSystem.recomputeVBO();
}

void App::switchFullScreenMode(){
	if (m_bFullScreen)
		SDL_SetWindowFullscreen(m_window, 0);
	else
		SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
	m_bFullScreen = !m_bFullScreen;
	onWindowResize();
}

/////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL CODE /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

App* App::m_instance = nullptr;

void App::Initialize(SDL_Window* window) {
	assert(!m_instance);
	m_instance = new App(100000, window);
	if (m_instance == nullptr)
		spdlog::error("[App::Initialize] Unable to allocate enough memory !");
}

void App::ShutDown() {
	delete m_instance;
}

void App::_loopIteration() {
	// Events
	handleSDLevents();
	// Start ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	// Actual application code
	onLoopIteration();

	// Render ImGui
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	// End frame
	SDL_GL_SwapWindow(m_window);
}

void App::handleSDLevents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		ImGui_ImplSDL2_ProcessEvent(&e);
		onEvent(e);
	}
}