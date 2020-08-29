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

void StateModifier::applyAllSettings() {
	ShaderPipeline& physicsCompute = m_particleSystem.physicsComputeShader();
	physicsCompute.bind();
	m_settingsManager.get().applyAndRecord(*this);
	physicsCompute.unbind();
}

void StateModifier::applyAndRecord(const StateChange& stateChange) {
	switch (stateChange.type) {
	case StateChangeType::Action:
		m_configManager.applyAndRecord_ActionRef(std::get<ActionRef>(stateChange.value), *this);
		break;
	case StateChangeType::AlphaTrail_Enabled:
		m_settingsManager.get().getTrail().setApplyAndRecord_Enabled(std::get<bool>(stateChange.value), *this);
		break;
	case StateChangeType::AlphaTrail_FixResiduals:
		m_settingsManager.get().getTrail().setFixResiduals(std::get<bool>(stateChange.value));
		m_settingsManager.get().getTrail().applyAndRecord_FixResiduals(*this);
		break;
	case StateChangeType::AlphaTrail_Decay:
		m_settingsManager.get().getTrail().setDecay(std::get<float>(stateChange.value));
		m_settingsManager.get().getTrail().applyAndRecord_Decay(*this);
		break;
	case StateChangeType::AlphaTrail_Threshold:
		m_settingsManager.get().getTrail().setThreshold(std::get<float>(stateChange.value));
		m_settingsManager.get().getTrail().applyAndRecord_Threshold(*this);
		break;
	case StateChangeType::AlphaTrail_MinAlpha:
		m_settingsManager.get().getTrail().setMinAlpha(std::get<float>(stateChange.value));
		m_settingsManager.get().getTrail().applyAndRecord_MinAlpha(*this);
		break;
	case StateChangeType::Color_Background:
		m_settingsManager.get().getColors().setBackgroundColor(std::get<glm::vec3>(stateChange.value));
		m_settingsManager.get().getColors().applyAndRecord_BackgroundColor(*this);
		break;
	case StateChangeType::Color_GradientMode:
		m_settingsManager.get().getColors().setGradientMode(std::get<bool>(stateChange.value));
		m_settingsManager.get().getColors().applyAndRecord_GradientMode(*this);
		break;
	case StateChangeType::Color_HueStart:
		m_settingsManager.get().getColors().setHueStart(std::get<float>(stateChange.value));
		m_settingsManager.get().getColors().applyAndRecord_HueStart(*this);
		break;
	case StateChangeType::Color_HueEnd:
		m_settingsManager.get().getColors().setHueEnd(std::get<float>(stateChange.value));
		m_settingsManager.get().getColors().applyAndRecord_HueEnd(*this);
		break;
	case StateChangeType::Color_Saturation:
		m_settingsManager.get().getColors().setSaturation(std::get<float>(stateChange.value));
		m_settingsManager.get().getColors().applyAndRecord_Saturation(*this);
		break;
	case StateChangeType::Color_Value:
		m_settingsManager.get().getColors().setValue(std::get<float>(stateChange.value));
		m_settingsManager.get().getColors().applyAndRecord_Value(*this);
		break;
	case StateChangeType::Color_ColorStart:
		m_settingsManager.get().getColors().setColorStart(std::get<glm::vec3>(stateChange.value));
		m_settingsManager.get().getColors().applyAndRecord_ColorStart(*this);
		break;
	case StateChangeType::Color_ColorEnd:
		m_settingsManager.get().getColors().setColorEnd(std::get<glm::vec3>(stateChange.value));
		m_settingsManager.get().getColors().applyAndRecord_ColorEnd(*this);
		break;
	case StateChangeType::Particles_Number:
		m_settingsManager.get().getPartSystem().setNbParticles(std::get<unsigned int>(stateChange.value));
		m_settingsManager.get().getPartSystem().applyAndRecord_NbParticles(*this);
		break;
	case StateChangeType::Particles_Radius:
		m_settingsManager.get().getPartSystem().setRadius(std::get<float>(stateChange.value));
		m_settingsManager.get().getPartSystem().applyAndRecord_ParticleRadius(*this);
		break;
	case StateChangeType::Physics_Pulsation:
		m_settingsManager.get().getPhysics().setPulsation(std::get<float>(stateChange.value));
		m_settingsManager.get().getPhysics().applyAndRecord_Pulsation(*this);
		break;
	case StateChangeType::Physics_DampingRatio:
		m_settingsManager.get().getPhysics().setDampingRatio(std::get<float>(stateChange.value));
		m_settingsManager.get().getPhysics().applyAndRecord_DampingRatio(*this);
		break;
	case StateChangeType::Wind_Frequency:
		m_settingsManager.get().getWind().setFrequency(std::get<float>(stateChange.value));
		m_settingsManager.get().getWind().applyAndRecord_Frequency(*this);
		break;
	case StateChangeType::Wind_MinStrength:
		m_settingsManager.get().getWind().setMinStrength(std::get<float>(stateChange.value));
		m_settingsManager.get().getWind().applyAndRecord_MinStrength(*this);
		break;
	case StateChangeType::Wind_MaxStrength:
		m_settingsManager.get().getWind().setMaxStrength(std::get<float>(stateChange.value));
		m_settingsManager.get().getWind().applyAndRecord_MaxStrength(*this);
		break;
	case StateChangeType::Wind_Speed:
		m_settingsManager.get().getWind().setSpeed(std::get<float>(stateChange.value));
		m_settingsManager.get().getWind().applyAndRecord_Speed(*this);
		break;
	case StateChangeType::Wind_Direction:
		m_settingsManager.get().getWind().setDirection(std::get<float>(stateChange.value));
		m_settingsManager.get().getWind().applyAndRecord_Direction(*this);
		break;
	case StateChangeType::ConfigParams:
		m_configManager.configParams().onKeyPressed(std::get<SDL_Scancode>(stateChange.value), *this);
		break;
	default:
		assert(false && "[StateModifier::applyAndRecord] Forgot a case !");
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