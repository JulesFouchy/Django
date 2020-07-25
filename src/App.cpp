#include "App.h"

#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include "Helper/DisplayInfos.h"
#include "Helper/Input.h"

#include "Configurations/ConfigFromSVG.h"

App::App(SDL_Window* window)
	: m_bShowImGUIDemoWindow(false),
	  m_bFullScreen(false),
	  m_bShowGUI(true),
	  m_particlesSystem(),
	  m_window(window), m_running(true)
{
	// Create graphics pipeline for particles
	m_particlePipeline.addShader(ShaderType::Vertex,   "internal-shaders/particle.vert");
	m_particlePipeline.addShader(ShaderType::Fragment, "internal-shaders/particle.frag");
	m_particlePipeline.createProgram();
	//
	onWindowResize();
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_BLEND); // This is already handled by Alpha Trail Settings
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void App::onInit() {
	m_particlesSystem.physicsComputeShader().bind();
	m_settingsMng.get().apply(m_particlesSystem.physicsComputeShader(), m_particlesSystem, m_configManager);
	m_particlesSystem.physicsComputeShader().unbind();
	CreateConfigFromSVG("C:/Users/Pc/Downloads/nanosvg-master/nanosvg-master/example/0410a6ceee93079243a1a0a0be387622.svg");
}

void App::onLoopIteration() {
	m_particlesSystem.physicsComputeShader().bind();
	// ImGui windows
	if (m_bShowGUI) {
#ifndef NDEBUG
		// Debug
		ImGui::Begin("Debug");
		ImGui::Checkbox("Show Demo Window", &m_bShowImGUIDemoWindow);
		ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);
		ImGui::End();
		if (m_bShowImGUIDemoWindow) // Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
			ImGui::ShowDemoWindow(&m_bShowImGUIDemoWindow);
#endif
		// Time
		// ImGui::Begin("Time");
		// ImGui::Text(("time : " + std::to_string(m_time.time())).c_str());
		// ImGui::Text(("FPS  : " + std::to_string(1.0f / m_time.deltaTime())).c_str());
		// ImGui::End();
		// Settings
		m_settingsMng.get().ImGuiWindows(m_particlesSystem.physicsComputeShader(), m_particlesSystem, m_configManager);
		m_configManager.Imgui(m_particlesSystem);
	}
	// Send time to physics compute shader
	m_time.update();
	m_particlesSystem.physicsComputeShader().setUniform1f("dt", m_time.deltaTime());
	// Send wind to physics compute shader
	m_settingsMng.get().getWind().setWindOffset(m_particlesSystem.physicsComputeShader(), m_time.time());
	// Send mouse to physics compute shader
		// Force field
	bool bForceField = Input::IsMouseButtonDown(SDL_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse;
	m_particlesSystem.physicsComputeShader().setUniform1i("u_bForceField", bForceField);
		// Burst
	bool bImpulse = ImGui::IsMouseReleased(ImGuiMouseButton_Right);
	if (bImpulse)
		m_particlesSystem.physicsComputeShader().setUniform1f("u_burstStrength", pow(25.0f * ImGui::GetIO().MouseDownDurationPrev[ImGuiMouseButton_Right], 0.8f));
	else
		m_particlesSystem.physicsComputeShader().setUniform1f("u_burstStrength", -1.0f);
		//
	if (bForceField || bImpulse)
		m_particlesSystem.physicsComputeShader().setUniform2f("u_mouse", Input::GetMouseInNormalizedRatioSpace());
	//
	m_particlesSystem.physicsComputeShader().unbind();
	// Clear screen
	m_settingsMng.get().getTrail().clearScreen(m_time.deltaTime(), m_settingsMng.get().getColors().backgroundColor());
	// Draw particles
	m_particlePipeline.bind();
	m_particlesSystem.draw();
	// Blit render buffer to screen if needed
	m_settingsMng.get().getTrail().finishRendering();
	// Update particles physics
	m_particlesSystem.updatePositions();
}

void App::onEvent(const SDL_Event& e) {
	switch (e.type) {

	case SDL_MOUSEMOTION:
		if (!ImGui::GetIO().WantCaptureMouse) {

		}
		break;

	case SDL_MOUSEWHEEL:
		m_configManager.onWheel(e.wheel.y, m_particlesSystem, ImGui::GetIO().WantCaptureMouse);
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
			if (e.key.keysym.sym == 'h' && Input::KeyIsDown(SDL_SCANCODE_LCTRL))
				m_bShowGUI = !m_bShowGUI;
			else {
				m_configManager.onKeyPressed(e.key, m_particlesSystem);
			}
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
	m_settingsMng.get().getTrail().setSize(DisplayInfos::Width(), DisplayInfos::Height());
	m_particlePipeline.bind();
	m_particlePipeline.setUniform1f("u_invAspectRatio", 1.0f / DisplayInfos::Ratio());
	glViewport(0, 0, DisplayInfos::Width(), DisplayInfos::Height());
	m_configManager.applyTo(m_particlesSystem); // some configs depend on the aspect ratio
	//m_particlesSystem.recomputeVBO();
}

void App::switchFullScreenMode(){
	if (m_bFullScreen)
		SDL_SetWindowFullscreen(m_window, 0);
	else
		SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
	m_bFullScreen = !m_bFullScreen;
	onWindowResize();
}

void App::beforeShutDown() {

}

/////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL CODE /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

App* App::m_instance = nullptr;

void App::Initialize(SDL_Window* window) {
	assert(!m_instance);
	m_instance = new App(window);
	if (m_instance == nullptr)
		spdlog::error("[App::Initialize] Unable to allocate enough memory !");
}

void App::ShutDown() {
	m_instance->beforeShutDown();
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