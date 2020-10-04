#include "App.h"

#include "Viewports/Viewports.h"
#include "Helper/Input.h"
#ifndef NDEBUG
#include "Helper/MyImGui.h"
#endif

App::App(GLWindow& mainGLWindow, GLWindow& outputGLWindow)
	: m_stateModifier(m_particleSystem, m_settingsManager, m_configManager, m_renderer, m_recordManager, m_mouseInteractions),
	  m_mainGLWindow(mainGLWindow), m_outputGLWindow(outputGLWindow)
{
	Viewports::setRenderSizeChangedCallback([this]() {onRenderSizeChanged(); });
	//
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_BLEND); // This is already handled by Alpha Trail Settings
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
	// Create graphics pipeline for particles
	m_particlePipeline.addShader(ShaderType::Vertex,   "internal-shaders/particle.vert");
	m_particlePipeline.addShader(ShaderType::Fragment, "internal-shaders/particle.frag");
	m_particlePipeline.createProgram();
	//
	m_settingsManager.get().partSystem().applyAndRecord_NbParticles(m_stateModifier); // Very important. Makes sure the partSystem SSBOs are initialized before applying any config
	//
	m_stateModifier.applyAndRecord_AllSettings();
	// Get output window size
	int x, y;
	SDL_GetWindowSize(m_outputGLWindow.window, &x, &y);
	Viewports::setOutputWindowSize(x, y);
	// Hide output window
	outputGLWindow.hide();
}

void App::update() {
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// ---------------------
	// ------ PHYSICS ------
	// ---------------------
	m_particleSystem.physicsComputeShader().bind();
	// Send time to physics compute shader
	m_recordManager.update(m_stateModifier); // updates time so must be called before sending it to compute shader // must be called after it's ImGui() because the latter is responsible for setting m_bDraggingOnTheTimeline
	m_particleSystem.physicsComputeShader().setUniform1f("dt", m_recordManager.clock().deltaTime());
	// Send wind to physics compute shader
	m_settingsManager.get().wind().setWindOffsetInShader(m_particleSystem.physicsComputeShader(), m_recordManager.clock().time());
	if (!Viewports::IsExporting()) {
		// Send mouse to physics compute shader
		m_mouseInteractions.update(m_stateModifier);
		// Move all particles towards mouse if wheel is down
		if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
			m_particleSystem.applyAndRecord_SetAllRestPositions(Input::GetMouseInNormalizedRatioSpace(), m_stateModifier);
	}
	// Apply physics
	m_particleSystem.updatePositions();
	m_particleSystem.physicsComputeShader().unbind();
	// ---------------------
	// ----- RENDERING -----
	// ---------------------
	// Clear screen
	m_renderer.onRenderBegin(m_recordManager.clock().deltaTime(), m_settingsManager.get().colors().backgroundColor(), m_settingsManager.get().alphaTrail().getValues());
	// Draw particles
	m_particlePipeline.bind();
	m_particleSystem.draw();
	// Blit render buffer to screen
	m_renderer.renderBuffer().blitToScreen(
		Viewports::SwapYConvention(Viewports::AppView().botLeft()),
		Viewports::SwapYConvention(Viewports::AppView().topRight())
	);
	// Render on output window
	if (Viewports::IsOutputWindowOpen()) {
		m_outputGLWindow.makeCurrent();
		m_renderer.renderBuffer().blitToScreen(
			{ 0, 0 },
			Viewports::getOutputWindowSize()
		);
		SDL_GL_SwapWindow(m_outputGLWindow.window);
		m_mainGLWindow.makeCurrent();
	}
	// Export
	if (Viewports::IsExporting())
		m_recordManager.exporter().exportFrame(m_renderer.renderBuffer());
	// ----------------------------
	// ---------- IMGUI -----------
	// ----------------------------
	// ImGui windows
	if (m_bShowGUI) {
		if (!Viewports::IsExporting()) {
			ImGui::BeginMainMenuBar();
			if (ImGui::BeginMenu("RenderAreas")) {
				ImGui::ColorEdit3("Empty space color", &m_clearColor[0]);
				bool bIsOutputWindowOpen = Viewports::IsOutputWindowOpen();
				if (ImGui::Checkbox("Show output window", &bIsOutputWindowOpen)) {
					setIsOutputWindowOpen(bIsOutputWindowOpen);
				}
				if (!bIsOutputWindowOpen) {
					Viewports::ImGuiConstrainAppViewRatio();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Windows")) {
				ImGui::Text("Hello !");
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
#ifndef NDEBUG
			// Debug
			ImGui::Begin("Debug");
			MyImGui::TimeFormatedHMS(m_recordManager.clock().time());
			ImGui::Checkbox("Show Demo Window", &m_bShowImGUIDemoWindow);
			ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);
			ImGui::Text("Render Size : %d %d", Viewports::RenderSize().width(), Viewports::RenderSize().height());
			ImGui::End();
			if (m_bShowImGUIDemoWindow) // Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
				ImGui::ShowDemoWindow(&m_bShowImGUIDemoWindow);
#endif
			// Settings
			m_stateModifier.settingsManager().get().ImGuiWindows(m_stateModifier);
			m_configManager.Imgui(m_stateModifier);
			// Key bindings
			ImGui::Begin("Key Bindings");
			m_configManager.ImGuiKeyBindings(m_stateModifier);
			ImGui::End();
		}
		// Recording
		ImGui::Begin("Recording");
		m_recordManager.ImGui(m_recordManager.clockPtrRef(), m_stateModifier); // must be called before m_recordManager.update()
		ImGui::End();
	}
}

void App::onEvent(const SDL_Event& e) {
	m_outputGLWindow.checkForFullscreenToggles(e);
	if (e.type == SDL_WINDOWEVENT && e.window.windowID == SDL_GetWindowID(m_outputGLWindow.window)) {
		switch (e.window.event) {
		case SDL_WINDOWEVENT_RESIZED:
			int x, y;
			SDL_GetWindowSize(m_outputGLWindow.window, &x, &y);
			Viewports::setOutputWindowSize(x, y);
			break;
		case SDL_WINDOWEVENT_CLOSE:
			setIsOutputWindowOpen(false);
			break;
		}
	}
	if (!Viewports::IsExporting()) {
		switch (e.type) {

		case SDL_MOUSEMOTION:
			if (!ImGui::GetIO().WantCaptureMouse) {

			}
			break;

		case SDL_MOUSEWHEEL:
			m_configManager.onWheel(e.wheel.y, ImGui::GetIO().WantCaptureMouse, m_stateModifier);
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
				if (e.button.button == SDL_BUTTON_LEFT) {

				}
				else if (e.button.button == SDL_BUTTON_RIGHT) {

				}
				else if (e.button.button == SDL_BUTTON_MIDDLE) {
					m_stateModifier.applyAndRecord_CurrentAction();
				}
			}
			break;


		case SDL_KEYDOWN:
			if (!ImGui::GetIO().WantTextInput) {
				if (e.key.keysym.sym == 'h' && Input::KeyIsDown(SDL_SCANCODE_LCTRL))
					m_bShowGUI = !m_bShowGUI;
				else {
					m_configManager.onKeyPressed(e.key.keysym.scancode, e.key.keysym.sym, m_stateModifier);
				}
			}
			break;

		case SDL_KEYUP:
			if (!ImGui::GetIO().WantTextInput) {
				if (!(e.key.keysym.sym == 'h') || !Input::KeyIsDown(SDL_SCANCODE_LCTRL))
					m_configManager.onKeyUp(e.key.keysym.scancode);
			}
			break;

		default:
			break;
		}
	}
}

void App::onRenderSizeChanged() {
	m_renderer.onRenderSizeChanged(m_settingsManager.get().colors().backgroundColor());
	m_particlePipeline.bind();
	m_particlePipeline.setUniform1f("u_invAspectRatio", 1.0f / Viewports::RenderSize().aspectRatio());
	m_stateModifier.apply(); // some configs depend on the aspect ratio 
}

void App::setIsOutputWindowOpen(bool isOpen) {
	Viewports::setIsOutputWindowOpen(isOpen);
	if (isOpen)
		m_outputGLWindow.show();
	else
		m_outputGLWindow.hide();
}