#include "ParticleSystemSettings.h"

#include "Particles/ParticlesSystem.h"
#include "Configurations/Configuration.h"
#include "ColorSettings.h"

ParticleSystemSettings::ParticleSystemSettings()
    : m_presets("djgParticles")
{}

void ParticleSystemSettings::ImGui(ParticlesSystem& partSystem, Configuration& currentConfiguration, const ColorSettingsValues& colorSettings) {
    bool b = false;
    // Nb of particles
    if (ImGui::SliderInt("Nb of particles", (int*)&m_values.nbParticles, 1, 100000)) {
        b = true;
        partSystem.setNbParticles(m_values.nbParticles, colorSettings);
        currentConfiguration.setup(m_values.nbParticles);
        currentConfiguration.applyTo(partSystem);
    }
    // Particles Radius
    if (ImGui::SliderFloat("Particles' Radius", &m_values.particleRadiusRelToHeight, 0.0f, 0.1f)) {
        b = true;
        partSystem.recomputeVBO(m_values.particleRadiusRelToHeight);
    }
    //
    if (m_presets.ImGui(&m_values)) {
        apply(partSystem, currentConfiguration, colorSettings);
    }
    if (b)
        m_presets.setToPlaceholderSetting();
}

void ParticleSystemSettings::apply(ParticlesSystem& partSystem, Configuration& currentConfiguration, const ColorSettingsValues& colorSettings) {
    // Nb particles
    partSystem.setNbParticles(m_values.nbParticles, colorSettings);
    currentConfiguration.setup(m_values.nbParticles);
    currentConfiguration.applyTo(partSystem);
    // Radius
    partSystem.recomputeVBO(m_values.particleRadiusRelToHeight);
}