#include "App.h"

#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <debug_break/debug_break.h>

#include "Clock/Clock_Realtime.h"

#ifdef _WIN32
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

int main(int argc, char *argv[]) {
	#ifdef _WIN32 // Check memory leaks
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif
		// ---------- Initialize spdlog ---------------

		Log::Initialize();

		// ------- Initialize SDL and OpenGL ------------

		SDL_Window* window;
		SDL_GLContext glContext;

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
			spdlog::critical("[SDL2] Unable to initialize SDL: {}", SDL_GetError());
			debug_break();
		}
		SDL_GL_LoadLibrary(NULL);
#if __APPLE__
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
#else
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
#endif
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

		window = SDL_CreateWindow(
			"Django",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			1280, 720,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
		);
		if (window == nullptr) {
			spdlog::critical("[SDL2] Window is null: {}", SDL_GetError());
			debug_break();
		}

		glContext = SDL_GL_CreateContext(window);
		if (glContext == nullptr) {
			spdlog::critical("[SDL2] OpenGL context is null: {}", SDL_GetError());
			debug_break();
		}

		SDL_GL_SetSwapInterval(1);

		if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
			spdlog::critical("[Glad] Glad not init");
			debug_break();
		}

		// ------- Initialize ImGUI ------------

#if __APPLE__
		const char* glslVersion = "#version 150";
#else
		const char* glslVersion = "#version 130";
#endif
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsClassic();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplSDL2_InitForOpenGL(window, glContext);
		ImGui_ImplOpenGL3_Init(glslVersion);

		// ------ Initialize our own classes

		Clock_Realtime::Initialize();
		App::Initialize(window);

		// ------ Actual App
		App::Get().onInit();
		while (App::Get().isRunning()) {
			App::Get()._loopIteration();
		}

		// ------ Shutdown
		App::ShutDown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
		SDL_DestroyWindow(window);
		SDL_Quit();

	return 0;
}