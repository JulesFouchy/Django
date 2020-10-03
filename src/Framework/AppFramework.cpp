#include "AppFramework.h"

#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include "imgui/imgui_internal.h"
#include "Viewports/Viewports.h"

AppFramework::AppFramework(SDL_Window* window, App& app)
	: m_window(window), m_app(app)
{
	Viewports::RenderArea.setSize(1, 1);
	onWindowMove();
	onWindowResize();
}

void AppFramework::onWindowMove() {
	int x, y;
	SDL_GetWindowPosition(m_window, &x, &y);
	Viewports::Window.setTopLeft(x, y);
}

void AppFramework::onWindowResize() {
	// Get new window size
	int w, h;
	SDL_GetWindowSize(m_window, &w, &h);
	// Update viewport infos
	Viewports::Window.setSize(w, h);
	// Update glViewport
	glViewport(0, 0, w, h);
	// Update app
	m_app.onWindowResize();
}

void AppFramework::switchFullScreenMode(){
	if (m_bFullScreen)
		SDL_SetWindowFullscreen(m_window, 0);
	else
		SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
	m_bFullScreen = !m_bFullScreen;
	onWindowResize();
}

void AppFramework::onEvent(SDL_Event e) {
	switch (e.type) {

	case SDL_WINDOWEVENT:
		switch (e.window.event) {
		case SDL_WINDOWEVENT_RESIZED:
			onWindowResize();
			break;
		case SDL_WINDOWEVENT_CLOSE:
			if (e.window.windowID == SDL_GetWindowID(m_window))
				exit();
			break;
			case SDL_WINDOWEVENT_MOVED:
				onWindowMove();
				break;
		}
		break;

	case SDL_KEYDOWN:
		if (e.key.keysym.scancode == SDL_SCANCODE_F11)
			switchFullScreenMode();
		if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE && m_bFullScreen)
			switchFullScreenMode();
		break;

	case SDL_QUIT:
		exit();
		break;
	}
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
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();
	dockspace();
	// Actual application code
	m_app.update();

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

void AppFramework::dockspace() {
	const ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

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
		ImGuiDockNode* node = ImGui::DockBuilderGetCentralNode(dockspace_id);
		auto h = Viewports::Window.height();
		ImVec2 pos = node->Pos;
		ImVec2 size = node->Size;
		int x = Viewports::Window.topLeft().x;
		int y = Viewports::Window.topLeft().y;
		Viewports::RenderArea.setTopLeft(pos.x - x, pos.y - y);
		if (size.x != Viewports::RenderArea.width() || size.y != Viewports::RenderArea.height()) {
			Viewports::RenderArea.setSize(size.x, size.y);
			m_app.onRenderAreaResize();
		}
	}
	else
	{
		spdlog::error("Docking not enabled !");
	}
	ImGui::End();
}