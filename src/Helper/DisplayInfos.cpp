#include "DisplayInfos.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>

int DisplayInfos::m_windowWidth;
int DisplayInfos::m_windowHeight;
float DisplayInfos::m_aspectRatio;

void DisplayInfos::RefreshSize(SDL_Window* window) {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	m_windowWidth = w;
	m_windowHeight = h;
	m_aspectRatio = w / (float) h;
}