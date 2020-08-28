#include "ParticleSystemSettings.h"

#include "StateModifier.h"

ParticleSystemSettings::ParticleSystemSettings()
    : m_presets("djgParticles")
{}

void ParticleSystemSettings::ImGui(StateModifier& stateModifier) {
    bool b = false;
    // Nb of particles
    if (ImGui::SliderInt("Nb of particles", (int*)&m_values.nbParticles, 1, 100000)) {
        b = true;
        stateModifier.setNbParticles(m_values.nbParticles);
        stateModifier.apply();
    }
    // Particles Radius
    if (ImGui::SliderFloat("Particles' Radius", &m_values.particleRadiusRelToHeight, 0.0f, 0.1f)) {
        b = true;
        stateModifier.setParticleRadius(m_values.particleRadiusRelToHeight);
        stateModifier.apply();
    }
    //
    if (m_presets.ImGui(&m_values)) {
        apply(stateModifier);
    }
    if (b)
        m_presets.setToPlaceholderSetting();
}

void ParticleSystemSettings::apply(StateModifier& stateModifier) {
    stateModifier.setNbParticles(m_values.nbParticles);
    stateModifier.setParticleRadius(m_values.particleRadiusRelToHeight);
    stateModifier.apply();
}