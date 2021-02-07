#include <Cool/Log/Log.h>
#include <Cool/App/AppManager.h>
#include <Cool/App/OpenGLWindowingSystem.h>
#include "Clock/Clock_Realtime.h"
#include "Constants/Textures.h"

#include "App.h"

int main() {
	// Init
	//Log::Initialize(); // TODO remove me
	Cool::Log::Initialize();
	Clock_Realtime::Initialize();
	// Init Glfw and set OpenGL version to 4.3
	Cool::OpenGLWindowingSystem windowingSystem(4, 3);
	// Create the windows and the shared OpenGL context
	Cool::OpenGLWindow outputWindow = windowingSystem.createWindow(
		"Django output",
		1280, 720
	);
	glfwHideWindow(outputWindow.get());
	Cool::OpenGLWindow mainWindow = windowingSystem.createWindow(
		__DJANGO_WINDOW_NAME, // Variable #defined in the CMake
		1280, 720
	);
#ifdef NDEBUG
	glfwMaximizeWindow(mainWindow.get());
#endif
	// Init
	Textures::_LoadAll();
	// App
	App app(mainWindow, outputWindow);
	Cool::AppManager appManager(mainWindow, app);
	appManager.run();
	// Shutdown
	Textures::_DestroyAll();
}