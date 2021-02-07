#include "App.h"

#include <Cool/App/RenderState.h>
#include <Cool/App/Input.h>
#ifndef NDEBUG
#include "Helper/MyImGui.h"
#endif

static void output_window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	OpenGLWindow* outputWindow = reinterpret_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window));
	outputWindow->checkForFullscreenToggles(key, scancode, action, mods);
}

App::App(OpenGLWindow& mainWindow, OpenGLWindow& outputWindow)
	: m_stateModifier(m_particleSystem, m_settingsManager, m_configManager, m_renderer, m_recordManager, m_mouseInteractions),
	  m_mainWindow(mainWindow), m_outputWindow(outputWindow)
{
	RenderState::SubscribeToSizeChanges([this]() {onRenderSizeChanged(); });
	// Output window
	glfwSetKeyCallback(m_outputWindow.get(), output_window_key_callback);
	glfwSetWindowUserPointer(m_outputWindow.get(), reinterpret_cast<void*>(&m_outputWindow));
	glfwSetWindowAttrib(m_outputWindow.get(), GLFW_AUTO_ICONIFY, GLFW_FALSE);
	glfwSetWindowAttrib(m_outputWindow.get(), GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
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
	int w, h;
	glfwGetWindowSize(m_outputWindow.get(), &w, &h);
	//Viewports::setOutputWindowSize(w, h);
}

void App::update() {
	if (!Input::KeyIsDown(GLFW_KEY_CAPS_LOCK)) {
		glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// ---------------------
		// ------ PHYSICS ------
		// ---------------------
		// Send time to physics compute shader
		m_recordManager.update(m_stateModifier); // updates time so must be called before sending it to compute shader // must be called after it's ImGui() because the latter is responsible for setting m_bDraggingOnTheTimeline
		m_particleSystem.physicsComputeShader().bind();
		m_particleSystem.physicsComputeShader().setUniform1f("dt", m_recordManager.clock().deltaTime());
		// Send wind to physics compute shader
		m_settingsManager.get().wind().setWindOffsetInShader(m_particleSystem.physicsComputeShader(), m_recordManager.clock().time());
		// Send mouse to physics compute shader
		m_mouseInteractions.update(m_stateModifier, RenderState::IsExporting());
		if (!RenderState::IsExporting()) {
			// Move all particles towards mouse if wheel is down
			if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
				m_particleSystem.applyAndRecord_SetAllRestPositions(Input::MouseInNormalizedRatioSpace(), m_stateModifier);
		}
		// Apply physics
		m_particleSystem.updatePositions();
		m_particleSystem.physicsComputeShader().unbind();
	}
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
		RenderState::SwapYConvention(RenderState::InAppRenderArea().botLeft()),
		RenderState::SwapYConvention(RenderState::InAppRenderArea().topRight())
	);
	// Render on output window
	if (glfwGetWindowAttrib(m_outputWindow.get(), GLFW_VISIBLE)) {
		m_outputWindow.makeCurrent();
		int w, h; glfwGetWindowSize(m_outputWindow.get(), &w, &h);
		m_renderer.renderBuffer().blitToScreen(
			{ 0, 0 },
			{ w, h }
		);
		glfwSwapBuffers(m_outputWindow.get());
		m_mainWindow.makeCurrent();
	}
	// Export
	if (RenderState::IsExporting())
		m_recordManager.exporter().exportFrame(m_renderer.renderBuffer());
	// ImGui Recording Window
	if (m_bOpenRecordings) {
		ImGui::Begin("Recording", &m_bOpenRecordings);
		m_recordManager.ImGui(m_recordManager.clockPtrRef(), m_stateModifier); // must be called before m_recordManager.update()
		ImGui::End();
	}
	//
	//glfwMakeContextCurrent(m_outputWindow.get());
	//glfwPollEvents();
	if (glfwWindowShouldClose(m_outputWindow.get())) {
		closeOutputWindow();
	}
	//glfwMakeContextCurrent(m_mainWindow.get());
}

void App::ImGuiWindows() {
	// Settings
	m_configManager.Imgui(m_stateModifier);
	m_stateModifier.settingsManager().get().ImGuiWindows(m_stateModifier);
	m_stateModifier.settingsManager().get().ImGuiMainWindow(m_stateModifier);
	// Key bindings
	if (m_bOpenKeyBindings) {
		ImGui::Begin("Key Bindings", &m_bOpenKeyBindings);
		m_configManager.ImGuiKeyBindings(m_stateModifier);
		ImGui::End();
	}
	// Configurations
	m_configManager.ImGuiWindow();
#ifndef NDEBUG
	// Debug
	if (m_bOpenDebug) {
		ImGui::Begin("Debug", &m_bOpenDebug);
		MyImGui::TimeFormatedHMS(m_recordManager.clock().time());
		ImGui::Checkbox("Show Demo Window", &m_bShowImGUIDemoWindow);
		ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("Render Size : %d %d", RenderState::Size().width(), RenderState::Size().height());
		ImGui::End();
	}
	if (m_bShowImGUIDemoWindow) // Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		ImGui::ShowDemoWindow(&m_bShowImGUIDemoWindow);
#endif
}

void App::ImGuiMenus() {
	if (ImGui::BeginMenu("RenderAreas")) {
		ImGui::ColorEdit3("Empty space color", &m_clearColor[0]);
		//if (!RenderState::IsOutputWindowOpen()) {
		//	RenderState::ImGuiConstrainAppViewRatio();
		//}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Windows")) {
		ImGui::Checkbox("Key Bindings", &m_bOpenKeyBindings);
		ImGui::Checkbox("Recordings", &m_bOpenRecordings);
		ImGui::Separator();
		m_stateModifier.settingsManager().get().ImGuiOpenWindowsCheckboxes();
		m_configManager.ImGuiOpenWindowCheckbox();
#ifndef NDEBUG
		ImGui::Separator();
		ImGui::Checkbox("Debug", &m_bOpenDebug);
#endif
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Live")) {
		LiveMode::ImGui();
		bool bIsOutputWindowOpen = glfwGetWindowAttrib(m_outputWindow.get(), GLFW_VISIBLE);
		if (ImGui::Checkbox("Show output window", &bIsOutputWindowOpen)) {
			if (bIsOutputWindowOpen) {
				openOutputWindow();
			}
			else {
				closeOutputWindow();
			}
		}
		if (LiveMode::ShowHelpMarkers()) {
			ImGui::SameLine();
			MyImGui::HelpMarker(R"V0G0N(Opens a second window showing only the output image. This is the one that you should project / stream for your audience.
)V0G0N");
		}
		ImGui::EndMenu();
	}
}

void App::onKeyboardEvent(int key, int scancode, int action, int mods) {
	if (!RenderState::IsExporting() && !ImGui::GetIO().WantTextInput) {

	}
}

void App::onMouseButtonEvent(int button, int action, int mods) {
	if (!RenderState::IsExporting() && !ImGui::GetIO().WantCaptureMouse) {

	}
}

void App::onScrollEvent(double xOffset, double yOffset) {
	if (!RenderState::IsExporting() && !ImGui::GetIO().WantCaptureMouse) {

	}
}

void App::onMouseMoveEvent(double xpos, double ypos) {
	if (!RenderState::IsExporting() && !ImGui::GetIO().WantCaptureMouse) {

	}
}

//void App::onEvent(const SDL_Event& e) {
	//if (e.type == SDL_WINDOWEVENT && e.window.windowID == SDL_GetWindowID(m_outputGLWindow.window)) {
	//	switch (e.window.event) {
	//	case SDL_WINDOWEVENT_RESIZED:
	//		int x, y;
	//		SDL_GetWindowSize(m_outputGLWindow.window, &x, &y);
	//		Viewports::setOutputWindowSize(x, y);
	//		break;
	//	case SDL_WINDOWEVENT_CLOSE:
	//		setIsOutputWindowOpen(false);
	//		break;
	//	}
	//}
	/*if (!RenderState::IsExporting()) {
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
				if (e.key.keysym.sym == 'h' && Input::CtrlOrCmdIsDown())
					m_bShowGUI = !m_bShowGUI;
				else {
					m_configManager.onKeyPressed(e.key.keysym.scancode, e.key.keysym.sym, m_stateModifier);
				}
			}
			break;

		case SDL_KEYUP:
			if (!ImGui::GetIO().WantTextInput) {
				if (!(e.key.keysym.sym == 'h') || !Input::CtrlOrCmdIsDown())
					m_configManager.onKeyUp(e.key.keysym.scancode);
			}
			break;

		default:
			break;
		}
	}*/
//}

void App::onRenderSizeChanged() {
	m_renderer.onRenderSizeChanged(m_settingsManager.get().colors().backgroundColor());
	m_particlePipeline.bind();
	m_particlePipeline.setUniform1f("u_invAspectRatio", 1.0f / RenderState::Size().aspectRatio());
	m_stateModifier.apply(); // some configs depend on the aspect ratio
}

void App::closeOutputWindow() {
	m_outputWindow.escapeFullScreen();
	glfwHideWindow(m_outputWindow.get());
}

void App::openOutputWindow() {
	glfwSetWindowShouldClose(m_outputWindow.get(), GLFW_FALSE);
	glfwShowWindow(m_outputWindow.get());
}