#include "Time_FixedTimestep.h"

Time_FixedTimestep::Time_FixedTimestep(float fps)
    : m_dt(1.0f / fps)
{}

float Time_FixedTimestep::deltaTime() const {
    return m_dt;
}

float Time_FixedTimestep::time() const {
    return m_framesCount * m_dt;
}

void Time_FixedTimestep::update() {
    m_framesCount++;
}