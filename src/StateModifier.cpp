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
		m_settingsManager.get().alphaTrail().setApplyAndRecord_Enabled(std::get<bool>(stateChange.value), *this);
		break;
	case StateChangeType::AlphaTrail_FixResiduals:
		m_settingsManager.get().alphaTrail().setFixResiduals(std::get<bool>(stateChange.value));
		m_settingsManager.get().alphaTrail().applyAndRecord_FixResiduals(*this);
		break;
	case StateChangeType::AlphaTrail_Decay:
		m_settingsManager.get().alphaTrail().setDecay(std::get<float>(stateChange.value));
		m_settingsManager.get().alphaTrail().applyAndRecord_Decay(*this);
		break;
	case StateChangeType::AlphaTrail_Threshold:
		m_settingsManager.get().alphaTrail().setThreshold(std::get<float>(stateChange.value));
		m_settingsManager.get().alphaTrail().applyAndRecord_Threshold(*this);
		break;
	case StateChangeType::AlphaTrail_MinAlpha:
		m_settingsManager.get().alphaTrail().setMinAlpha(std::get<float>(stateChange.value));
		m_settingsManager.get().alphaTrail().applyAndRecord_MinAlpha(*this);
		break;
	case StateChangeType::Color_Background:
		m_settingsManager.get().colors().setBackgroundColor(std::get<glm::vec3>(stateChange.value));
		m_settingsManager.get().colors().applyAndRecord_BackgroundColor(*this);
		break;
	case StateChangeType::Color_GradientMode:
		m_settingsManager.get().colors().setGradientMode(std::get<bool>(stateChange.value));
		m_settingsManager.get().colors().applyAndRecord_GradientMode(*this);
		break;
	case StateChangeType::Color_HueStart:
		m_settingsManager.get().colors().setHueStart(std::get<float>(stateChange.value));
		m_settingsManager.get().colors().applyAndRecord_HueStart(*this);
		break;
	case StateChangeType::Color_HueEnd:
		m_settingsManager.get().colors().setHueEnd(std::get<float>(stateChange.value));
		m_settingsManager.get().colors().applyAndRecord_HueEnd(*this);
		break;
	case StateChangeType::Color_Saturation:
		m_settingsManager.get().colors().setSaturation(std::get<float>(stateChange.value));
		m_settingsManager.get().colors().applyAndRecord_Saturation(*this);
		break;
	case StateChangeType::Color_Value:
		m_settingsManager.get().colors().setValue(std::get<float>(stateChange.value));
		m_settingsManager.get().colors().applyAndRecord_Value(*this);
		break;
	case StateChangeType::Color_ColorStart:
		m_settingsManager.get().colors().setColorStart(std::get<glm::vec3>(stateChange.value));
		m_settingsManager.get().colors().applyAndRecord_ColorStart(*this);
		break;
	case StateChangeType::Color_ColorEnd:
		m_settingsManager.get().colors().setColorEnd(std::get<glm::vec3>(stateChange.value));
		m_settingsManager.get().colors().applyAndRecord_ColorEnd(*this);
		break;
	case StateChangeType::Particles_Number:
		m_settingsManager.get().partSystem().setNbParticles(std::get<unsigned int>(stateChange.value));
		m_settingsManager.get().partSystem().applyAndRecord_NbParticles(*this);
		break;
	case StateChangeType::Particles_Radius:
		m_settingsManager.get().partSystem().setRadius(std::get<float>(stateChange.value));
		m_settingsManager.get().partSystem().applyAndRecord_ParticleRadius(*this);
		break;
	case StateChangeType::Physics_Pulsation:
		m_settingsManager.get().physics().setPulsation(std::get<float>(stateChange.value));
		m_settingsManager.get().physics().applyAndRecord_Pulsation(*this);
		break;
	case StateChangeType::Physics_DampingRatio:
		m_settingsManager.get().physics().setDampingRatio(std::get<float>(stateChange.value));
		m_settingsManager.get().physics().applyAndRecord_DampingRatio(*this);
		break;
	case StateChangeType::Wind_Frequency:
		m_settingsManager.get().wind().setFrequency(std::get<float>(stateChange.value));
		m_settingsManager.get().wind().applyAndRecord_Frequency(*this);
		break;
	case StateChangeType::Wind_MinStrength:
		m_settingsManager.get().wind().setMinStrength(std::get<float>(stateChange.value));
		m_settingsManager.get().wind().applyAndRecord_MinStrength(*this);
		break;
	case StateChangeType::Wind_MaxStrength:
		m_settingsManager.get().wind().setMaxStrength(std::get<float>(stateChange.value));
		m_settingsManager.get().wind().applyAndRecord_MaxStrength(*this);
		break;
	case StateChangeType::Wind_Speed:
		m_settingsManager.get().wind().setSpeed(std::get<float>(stateChange.value));
		m_settingsManager.get().wind().applyAndRecord_Speed(*this);
		break;
	case StateChangeType::Wind_Direction:
		m_settingsManager.get().wind().setDirection(std::get<float>(stateChange.value));
		m_settingsManager.get().wind().applyAndRecord_Direction(*this);
		break;
	case StateChangeType::Random_Seed:
		m_configManager.randParams().setApplyAndRecord_Seed(std::get<float>(stateChange.value), *this);
		break;
	case StateChangeType::Random_XYSeed:
		m_configManager.randParams().setApplyAndRecord_SeedXY(std::get<glm::vec2>(stateChange.value), *this);
		break;
	case StateChangeType::Param_KeyPressed:
		m_configManager.configParams().onKeyPressed(std::get<SDL_Scancode>(stateChange.value), *this);
		break;
	case StateChangeType::Param_Wheel:
		m_configManager.configParams().setApplyAndRecord_Wheel(std::get<float>(stateChange.value), *this);
		break;
	case StateChangeType::Param_CtrlWheel:
		m_configManager.configParams().setApplyAndRecord_CtrlWheel(std::get<float>(stateChange.value), *this);
		break;
	case StateChangeType::Param_ShiftWheel:
		m_configManager.configParams().setApplyAndRecord_ShiftWheel(std::get<float>(stateChange.value), *this);
		break;
	case StateChangeType::Param_AltWheel:
		m_configManager.configParams().setApplyAndRecord_AltWheel(std::get<float>(stateChange.value), *this);
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