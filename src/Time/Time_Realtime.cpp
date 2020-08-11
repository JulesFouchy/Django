#include "Time_Realtime.h"

float Time_Realtime::SDLPerformanceCounter2Seconds;

void Time_Realtime::Initialize() {
    SDLPerformanceCounter2Seconds = 1.0f / SDL_GetPerformanceFrequency();
}

Time_Realtime::Time_Realtime() 
    : m_lastTime(SDL_GetPerformanceCounter()), m_currentTime(m_lastTime)
{}

float Time_Realtime::deltaTime() {
    return (m_currentTime - m_lastTime) * SDLPerformanceCounter2Seconds;
}

float Time_Realtime::time() {
	return m_currentTime * SDLPerformanceCounter2Seconds;
}

void Time_Realtime::update(){
    m_lastTime = m_currentTime;
    m_currentTime = SDL_GetPerformanceCounter();
}