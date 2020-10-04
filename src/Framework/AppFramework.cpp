#include "AppFramework.h"

#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include "imgui/imgui_internal.h"
#include "Viewports/Viewports.h"

AppFramework::AppFramework(GLWindow& glWindow, App& app)
	: m_glWindow(glWindow), m_app(app)
{
	onWindowMove();
	onWindowResize();
}

void AppFramework::onWindowMove() {
	int x, y;
	SDL_GetWindowPosition(m_glWindow.window, &x, &y);
	Viewports::Window.setTopLeft(x, y);
}

void AppFramework::onWindowResize() {
	int w, h;
	SDL_GetWindowSize(m_glWindow.window, &w, &h);
	Viewports::Window.setSize(w, h);
	glViewport(0, 0, w, h);
}

void AppFramework::updateRenderArea(ImGuiDockNode* node) {
	// Position
	Viewports::RenderArea.setUnconstrainedTopLeft(
		node->Pos.x - Viewports::Window.topLeft().x,
		node->Pos.y - Viewports::Window.topLeft().y
	);
	// Size
	glm::ivec2 size = { static_cast<int>(node->Size.x), static_cast<int>(node->Size.y) };
	if (size.x != Viewports::RenderArea.unconstrainedSize().x || size.y != Viewports::RenderArea.unconstrainedSize().y) {
		Viewports::RenderArea.setUnconstrainedSize(size.x, size.y);
		m_app.onRenderAreaResized();
	}
}

void AppFramework::onEvent(const SDL_Event& e) {
	if (m_glWindow.checkForFullscreenToggles(e))
		onWindowResize();
	switch (e.type) {

	case SDL_WINDOWEVENT:
		switch (e.window.event) {
		case SDL_WINDOWEVENT_RESIZED:
			onWindowResize();
			break;
		case SDL_WINDOWEVENT_CLOSE:
			if (e.window.windowID == SDL_GetWindowID(m_glWindow.window))
				exit();
			break;
			case SDL_WINDOWEVENT_MOVED:
				onWindowMove();
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

void AppFramework::ImGuiDockspace() {
	const ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground
									| ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
									| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
									| ImGuiWindowFlags_NoNavFocus;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	bool bopen = true;
	ImGui::Begin("MyMainDockSpace", &bopen, window_flags);
	ImGui::PopStyleVar(3);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		updateRenderArea(ImGui::DockBuilderGetCentralNode(dockspace_id));
	}
	else
	{
		spdlog::warn("Docking not enabled !");
	}
	ImGui::End();
}

void AppFramework::update() {
	// Events
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		ImGui_ImplSDL2_ProcessEvent(&e);
		onEvent(e);
		m_app.onEvent(e);
	}
	// Start ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_glWindow.window);
	ImGui::NewFrame();
	ImGuiDockspace();
	// Actual application code
	m_app.update();
	// Render ImGui
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	// End frame
	SDL_GL_SwapWindow(m_glWindow.window);
}