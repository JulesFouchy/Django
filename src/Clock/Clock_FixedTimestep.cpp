#include "Clock_FixedTimestep.h"

Clock_FixedTimestep::Clock_FixedTimestep(float fps)
    : m_dt(1.0f / fps)
{}

float Clock_FixedTimestep::deltaTime() const {
    return m_dt;
}

float Clock_FixedTimestep::time() const {
    return m_framesCount * m_dt;
}

void Clock_FixedTimestep::update() {
    m_framesCount++;
}