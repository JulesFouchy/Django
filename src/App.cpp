#include "App.h"

#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include "Helper/DisplayInfos.h"
#include "Helper/Input.h"
#ifndef NDEBUG
#include "Helper/MyImGui.h"
#endif

App::App(SDL_Window* window)
	: m_bShowImGUIDemoWindow(false),
	  m_bFullScreen(false),
	  m_bShowGUI(true),
	  m_renderer([this](){onRenderTargetModified();}),
	  m_stateModifier(m_particleSystem, m_settingsManager, m_configManager, m_renderer, m_recordManager, m_mouseInteractions),
	  m_window(window), m_running(true)
{
	// Create graphics pipeline for particles
	m_particlePipeline.addShader(ShaderType::Vertex,   "internal-shaders/particle.vert");
	m_particlePipeline.addShader(ShaderType::Fragment, "internal-shaders/particle.frag");
	m_particlePipeline.createProgram();
	//
	m_settingsManager.get().partSystem().applyAndRecord_NbParticles(m_stateModifier); // Very important. Makes sure the partSystem SSBOs are initialized before applying any config
	onWindowResize(); // Also applies the current config
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_BLEND); // This is already handled by Alpha Trail Settings
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
	//
	m_stateModifier.applyAndRecord_AllSettings();
}

void App::onLoopIteration() {
	// ---------------------
	// ------ DOCKING ------
	// ---------------------
	dockspace();
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
		// Move all particles towars mouse if wheel is down
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
			if (e.key.keysym.scancode == SDL_SCANCODE_F11)
				switchFullScreenMode();
			if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE && m_bFullScreen)
				switchFullScreenMode();
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

		case SDL_WINDOWEVENT:
			switch (e.window.event) {
			case SDL_WINDOWEVENT_RESIZED:
				onWindowResize();
				break;
			case SDL_WINDOWEVENT_CLOSE:
				if (e.window.windowID == SDL_GetWindowID(m_window))
					exit();
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
}

void App::onWindowResize() {
	DisplayInfos::RefreshSize(m_window);
	glViewport(0, 0, DisplayInfos::Width(), DisplayInfos::Height());
	m_renderer.onWindowResize(DisplayInfos::Width(), DisplayInfos::Height());
	onRenderTargetModified();
}

void App::onRenderTargetModified() {
	m_particlePipeline.bind();
	m_particlePipeline.setUniform1f("u_invAspectRatio", 1.0f / m_renderer.aspectRatio());
	m_stateModifier.apply(); // some configs depend on the aspect ratio 
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

void App::_loopIteration() {
	// Events
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		ImGui_ImplSDL2_ProcessEvent(&e);
		onEvent(e);
	}
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

void App::dockspace() {
	const ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	bool bopen = true;
	ImGui::Begin("DockSpace Demo", &bopen, window_flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	else
	{

	}
	ImGui::End();
}