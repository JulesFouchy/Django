#include "Time_Realtime.h"

#include <SDL2/SDL.h>

float Time_Realtime::SDLPerformanceFrequency;

void Time_Realtime::Initialize() {
    SDLPerformanceFrequency = (float)SDL_GetPerformanceFrequency();
}

float Time_Realtime::deltaTime() {
    return (m_currentTime - m_lastTime) / SDLPerformanceFrequency;
}

float Time_Realtime::time() {
	return SDL_GetTicks() * 0.001f;
}

void Time_Realtime::update(){
    m_lastTime = m_currentTime;
    m_currentTime = SDL_GetPerformanceCounter();
}