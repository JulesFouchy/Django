#pragma once

struct GLWindow {
	GLWindow(SDL_Window* _window)
		: window(_window) 
	{
		glContext = SDL_GL_CreateContext(window);
		if (glContext == nullptr) {
			spdlog::critical("[SDL2] OpenGL context is null: {}", SDL_GetError());
			debug_break();
		}
	}
	SDL_Window* window;
	SDL_GLContext glContext;

	inline void makeCurrent() { SDL_GL_MakeCurrent(window, glContext); }
	inline void destroy() { SDL_DestroyWindow(window); }
};