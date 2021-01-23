#include "ParticleSystemSettings.h"

#include "StateModifier.h"
#include "Particles/ParticlesSystem.h"
#include "Settings/SettingsManager.h"
#include "Recording/StateChange.h"

ParticleSystemSettings::ParticleSystemSettings()
    : m_presets("djgParticles")
{}

void ParticleSystemSettings::ImGui(StateModifier& stateModifier) {
    // Nb of particles
    if (ImGui::SliderInt("Nb of particles", (int*)&m_values.nbParticles, 1, 100000)) {
        m_presets.setToPlaceholderSetting();
        applyAndRecord_NbParticles(stateModifier);
    }
    // Particles Radius
    if (ImGui::SliderFloat("Particles' Radius", &m_values.particleRadiusRelToHeight, 0.0f, 0.1f)) {
        m_presets.setToPlaceholderSetting();
        applyAndRecord_ParticleRadius(stateModifier);
    }
    //
    if (m_presets.ImGui(&m_values))
        applyAndRecord(stateModifier);
}

void ParticleSystemSettings::ImGuiPresets(StateModifier& stateModifier) {
    if (m_presets.ImGuiDropdown("Particles", &m_values)) {
        applyAndRecord(stateModifier);
    }
}

void ParticleSystemSettings::applyAndRecord(StateModifier& stateModifier) {
    applyAndRecord_NbParticles(stateModifier);
    applyAndRecord_ParticleRadius(stateModifier);
}

void ParticleSystemSettings::applyAndRecord_NbParticles(StateModifier& stateModifier) {
    stateModifier.particleSystem().applyNbParticles(
        m_values.nbParticles,
        stateModifier.settingsManager().get().colors().getValues()
    );
    stateModifier.apply(); // adapt configuration to the new number of particles
    stateModifier.recordChange({ StateChangeType::Particles_Number, m_values.nbParticles });
}

void ParticleSystemSettings::applyAndRecord_ParticleRadius(StateModifier& stateModifier) {
    stateModifier.particleSystem().recomputeVBO(m_values.particleRadiusRelToHeight);
    stateModifier.recordChange({ StateChangeType::Particles_Radius, m_values.particleRadiusRelToHeight });
}