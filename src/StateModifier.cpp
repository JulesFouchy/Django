#include "StateModifier.h"

#include "Particles/ParticlesSystem.h"
#include "Settings/SettingsManager.h"
#include "Configurations/ConfigManager.h"
#include "Renderer.h"
#include "Recording/RecordManager.h"
#include "Recording/StateChange.h"
#include "Actions/Action.h"

StateModifier::StateModifier(ParticleSystem& particleSystem, SettingsManager& settingsManager, ConfigManager& configManager, Renderer& renderer, RecordManager& recordManager)
	: m_particleSystem(particleSystem),
	  m_settingsManager(settingsManager),
	  m_configManager(configManager),
	  m_renderer(renderer),
	  m_recordManager(recordManager)
{}

void StateModifier::setNbParticles(unsigned int nbParticles) {
	m_particleSystem.setNbParticles(nbParticles, m_settingsManager.get().getColors().getValues());
}

void StateModifier::setParticleRadius(float radiusRelToHeight) {
	m_particleSystem.recomputeVBO(radiusRelToHeight);
}

void StateModifier::applyAllSettings() {
	ShaderPipeline& physicsCompute = m_particleSystem.physicsComputeShader();
	physicsCompute.bind();
	m_settingsManager.get().apply(*this);
	physicsCompute.unbind();
}

void StateModifier::apply(const StateChange& stateChange) {

}

void StateModifier::apply(const State& state) {

}

void StateModifier::apply(const Action& action) {

}

void StateModifier::apply() {
	m_configManager.applyTo(m_particleSystem);
}

void StateModifier::recordChange(const StateChange& stateChange) {
	m_recordManager.onStateChange(stateChange);
}