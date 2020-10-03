#include "App.h"

#include "Viewports/Viewports.h"
#include "Helper/Input.h"
#ifndef NDEBUG
#include "Helper/MyImGui.h"
#endif

App::App()
	: m_renderer([this](){onRenderTargetModified();}),
	  m_stateModifier(m_particleSystem, m_settingsManager, m_configManager, m_renderer, m_recordManager, m_mouseInteractions)
{
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
}

void App::update() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
	if (!m_recordManager.isExporting()) {
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
	// Blit render buffer to screen if needed
	m_renderer.onRenderEnd(m_settingsManager.get().alphaTrail().getValues());
	// Export
	if (m_recordManager.exporter().isExporting())
		m_recordManager.exporter().exportFrame();
	// ----------------------------
	// ---------- IMGUI -----------
	// ----------------------------
	// ImGui windows
	if (m_bShowGUI) {
		if (!m_recordManager.isExporting()) {
#ifndef NDEBUG
			// Debug
			ImGui::Begin("Debug");
			MyImGui::TimeFormatedHMS(m_recordManager.clock().time());
			ImGui::Checkbox("Show Demo Window", &m_bShowImGUIDemoWindow);
			ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);
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
	if (!m_recordManager.isExporting()) {
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

void App::onRenderAreaResized() {
	m_renderer.onWindowResize(Viewports::RenderArea.width(), Viewports::RenderArea.height());
	onRenderTargetModified();
}

void App::onRenderTargetModified() {
	m_particlePipeline.bind();
	m_particlePipeline.setUniform1f("u_invAspectRatio", 1.0f / Viewports::RenderArea.aspectRatio());
	m_stateModifier.apply(); // some configs depend on the aspect ratio 
}