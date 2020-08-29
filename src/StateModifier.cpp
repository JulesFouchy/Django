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

void StateModifier::applyAndRecord(const StateChange& stateChange) {
	switch (stateChange.type) {
	case StateChangeType::Action:
		m_configManager.applyAndRecord_ActionRef(std::get<ActionRef>(stateChange.value), *this);
		break;
	case StateChangeType::Physics_Pulsation:
		m_settingsManager.get().getPhysics().setPulsation(std::get<float>(stateChange.value));
		m_settingsManager.get().getPhysics().applyAndRecord_Pulsation(*this);
		break;
	case StateChangeType::Physics_DampingRatio:
		m_settingsManager.get().getPhysics().setDampingRatio(std::get<float>(stateChange.value));
		m_settingsManager.get().getPhysics().applyAndRecord_DampingRatio(*this);
		break;
	case StateChangeType::ConfigParams:
		m_configManager.configParams().onKeyPressed(std::get<SDL_Scancode>(stateChange.value), *this);
		break;
	}
}

void StateModifier::applyAndRecord(const State& state) {

}

void StateModifier::apply() {
	m_configManager.applyTo(m_particleSystem);
}

void StateModifier::recordChange(const StateChange& stateChange) {
	m_recordManager.recordChange(stateChange);
}