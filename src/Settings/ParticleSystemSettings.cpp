#include "ParticleSystemSettings.h"

#include "Particles/ParticlesSystem.h"
#include "Configurations/Configuration.h"

void ParticleSystemSettings::ImGui(ParticlesSystem& partSystem, Configuration& currentConfiguration) {
    // Nb of particles
    if (ImGui::SliderInt("Nb of particles", (int*)&m_nbParticles, 1, 100000)) {
        partSystem.setNbParticles(m_nbParticles);
        currentConfiguration.setup(m_nbParticles);
        currentConfiguration.applyTo(partSystem);
    }
    // Particles Radius
    if (ImGui::SliderFloat("Particles' Radius", &m_particleRadiusRelToHeight, 0.0f, 0.1f))
        partSystem.recomputeVBO(m_particleRadiusRelToHeight);
    //
}

void ParticleSystemSettings::apply(ParticlesSystem& partSystem, Configuration& currentConfiguration) {
    // Nb particles
    partSystem.setNbParticles(m_nbParticles);
    currentConfiguration.setup(m_nbParticles);
    currentConfiguration.applyTo(partSystem);
    // Radius
    partSystem.recomputeVBO(m_particleRadiusRelToHeight);
}