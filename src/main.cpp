#include "Framework/AppFramework.h"
#include "Framework/GLWindow.h"

#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <debug_break/debug_break.h>
#include "Framework/GlDebugCallback.h"

#include "Clock/Clock_Realtime.h"
#include "Constants/Textures.h"

#include <nfd.hpp>

#if defined(_MSC_VER) && defined(NDEBUG)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#ifdef _WIN32
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

int main(int argc, char *argv[]) {
	#ifdef _WIN32 // Check memory leaks
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif
		// ---------- Initialize nfd ---------------

		NFD::Guard nfdGuard;

		// ---------- Initialize spdlog ---------------

		Log::Initialize();

		// ------- Initialize SDL and OpenGL ------------

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
		SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
#ifndef NDEBUG
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

		// Output window
		SDL_Window* outputWindow = SDL_CreateWindow(
			"Django output",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			1280, 720,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN
		);
		if (outputWindow == nullptr) {
			spdlog::critical("[SDL2] Window is null: {}", SDL_GetError());
			debug_break();
		}
		GLWindow outputGLWindow(outputWindow);
		// Main window
		SDL_Window* mainWindow = SDL_CreateWindow(
			"Django",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			1280, 720,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI 
#ifdef NDEBUG
			| SDL_WINDOW_MAXIMIZED
#endif
		);
		if (mainWindow == nullptr) {
			spdlog::critical("[SDL2] Window is null: {}", SDL_GetError());
			debug_break();
		}
		GLWindow mainGLWindow(mainWindow, outputGLWindow.glContext);
		mainGLWindow.makeCurrent();
		//
		SDL_GL_SetSwapInterval(1);

		if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
			spdlog::critical("[Glad] Glad not init");
			debug_break();
		}

		// OpenGL Debugging
#ifndef NDEBUG
		int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(GLDebugCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
		else {
			spdlog::warn("Couldn't setup OpenGL Debugging");
		}
#endif

		// ------- Initialize ImGUI ------------

		const char* glslVersion = "#version 430";

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

		ImGui_ImplSDL2_InitForOpenGL(mainGLWindow.window, mainGLWindow.glContext);
		ImGui_ImplOpenGL3_Init(glslVersion);

		// ------ Initialize our own classes

		Clock_Realtime::Initialize();
		Textures::_LoadAll();

		// ------ Actual App
		{
			App app(mainGLWindow, outputGLWindow);
			AppFramework appFramework(mainGLWindow, app);
			while (appFramework.isRunning()) {
				appFramework.update();
			}
		} // Needs to be in a scope so that app is destructed here. Otherwise console won't close

		// ------ Shutdown
		Textures::_DestroyAll();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
		mainGLWindow.destroy();
		outputGLWindow.destroy();
		SDL_Quit();

	return 0;
}