#include "StateModifier.h"

#include "Particles/ParticlesSystem.h"
#include "Settings/SettingsManager.h"
#include "Configurations/ConfigManager.h"
#include "Renderer.h"
#include "Recording/RecordManager.h"
#include "Recording/StateChange.h"
#include "Actions/Action.h"
#include "MouseInteractions.h"

StateModifier::StateModifier(ParticleSystem& particleSystem, SettingsManager& settingsManager, ConfigManager& configManager, Renderer& renderer, RecordManager& recordManager, MouseInteractions& mouseInteractions)
	: m_particleSystem(particleSystem),
	  m_settingsManager(settingsManager),
	  m_configManager(configManager),
	  m_renderer(renderer),
	  m_recordManager(recordManager),
	  m_mouseInteractions(mouseInteractions)
{}

void StateModifier::applyAndRecord_AllSettings() {
	ShaderPipeline& physicsCompute = m_particleSystem.physicsComputeShader();
	physicsCompute.bind();
	m_settingsManager.get().applyAndRecord(*this);
	physicsCompute.unbind();
}


void StateModifier::applyAndRecord_CurrentAction() {
	m_configManager.applyAndRecord_ActionRef(m_configManager.getCurrentConfigAsActionRef(), *this);
}

void StateModifier::setAndRecord(const StateChange& stateChange) {
	m_bBlockApplying = true; // Ugly thing because I thought of it too late and can't be bothered to create all the setAndRecord functions
	setApplyAndRecord(stateChange);
	m_bBlockApplying = false;
}

void StateModifier::setApplyAndRecord(const StateChange& stateChange) {
	switch (stateChange.type) {
	case StateChangeType::Action:
		m_configManager.applyAndRecord_ActionRef(std::get<ActionRef>(stateChange.value), *this);
		break;
	case StateChangeType::Text:
		m_configManager.textConfig().setText(std::get<std::string>(stateChange.value));
		recordChange({ StateChangeType::Text, std::get<std::string>(stateChange.value) });
		break;
	case StateChangeType::Text_AddChar:
		m_configManager.textConfig().setApplyAndRecord_AddOneChar(std::get<char>(stateChange.value), *this);
		break;
	case StateChangeType::Text_SupprChar:
		m_configManager.textConfig().setApplyAndRecord_SupprOneChar(*this);
		break;
	case StateChangeType::Text_SupprAll:
		m_configManager.textConfig().setApplyAndRecord_SupprAllChars(*this);
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
	case StateChangeType::AlphaTrail_PresetName:
		m_settingsManager.get().alphaTrail().setPresetName(std::get<std::string>(stateChange.value));
		m_settingsManager.get().alphaTrail().applyAndRecord_PresetName(*this);
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
	case StateChangeType::Color_PresetName:
		m_settingsManager.get().colors().setPresetName(std::get<std::string>(stateChange.value));
		m_settingsManager.get().colors().applyAndRecord_PresetName(*this);
		break;
	case StateChangeType::Particles_Number:
		m_settingsManager.get().partSystem().setNbParticles(std::get<unsigned int>(stateChange.value));
		m_settingsManager.get().partSystem().applyAndRecord_NbParticles(*this);
		break;
	case StateChangeType::Particles_Radius:
		m_settingsManager.get().partSystem().setRadius(std::get<float>(stateChange.value));
		m_settingsManager.get().partSystem().applyAndRecord_ParticleRadius(*this);
		break;
	case StateChangeType::Particles_PresetName:
		m_settingsManager.get().partSystem().setPresetName(std::get<std::string>(stateChange.value));
		m_settingsManager.get().partSystem().applyAndRecord_PresetName(*this);
		break;
	case StateChangeType::Physics_Pulsation:
		m_settingsManager.get().physics().setPulsation(std::get<float>(stateChange.value));
		m_settingsManager.get().physics().applyAndRecord_Pulsation(*this);
		break;
	case StateChangeType::Physics_DampingRatio:
		m_settingsManager.get().physics().setDampingRatio(std::get<float>(stateChange.value));
		m_settingsManager.get().physics().applyAndRecord_DampingRatio(*this);
		break;
	case StateChangeType::Physics_PresetName:
		m_settingsManager.get().physics().setPresetName(std::get<std::string>(stateChange.value));
		m_settingsManager.get().physics().applyAndRecord_PresetName(*this);
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
	case StateChangeType::Wind_PresetName:
		m_settingsManager.get().wind().setPresetName(std::get<std::string>(stateChange.value));
		m_settingsManager.get().wind().applyAndRecord_PresetName(*this);
		break;
	case StateChangeType::Random:
		m_configManager.setRandParams(std::get<RandomParams>(stateChange.value));
		recordChange({ StateChangeType::Random, std::get<RandomParams>(stateChange.value) });
		break;
	case StateChangeType::Random_Seed:
		m_configManager.randParams().setApplyAndRecord_Seed(std::get<float>(stateChange.value), *this);
		break;
	case StateChangeType::Random_XYSeed:
		m_configManager.randParams().setApplyAndRecord_SeedXY(std::get<glm::vec2>(stateChange.value), *this);
		break;
	case StateChangeType::Param:
		m_configManager.setConfigParams(std::get<ConfigParams>(stateChange.value));
		recordChange({ StateChangeType::Param, std::get<ConfigParams>(stateChange.value) });
		break;
	case StateChangeType::Param_KeyPressed:
		m_configManager.configParams().onKeyPressed(std::get<SDL_Scancode>(stateChange.value), *this);
		break;
	case StateChangeType::Param_Wheel:
		m_configManager.configParams().setApplyAndRecord_Wheel(std::get<int>(stateChange.value), *this);
		break;
	case StateChangeType::Param_CtrlWheel:
		m_configManager.configParams().setApplyAndRecord_CtrlWheel(std::get<int>(stateChange.value), *this);
		break;
	case StateChangeType::Param_ShiftWheel:
		m_configManager.configParams().setApplyAndRecord_ShiftWheel(std::get<int>(stateChange.value), *this);
		break;
	case StateChangeType::Param_AltWheel:
		m_configManager.configParams().setApplyAndRecord_AltWheel(std::get<int>(stateChange.value), *this);
		break;
	case StateChangeType::Mouse_ForceField:
		m_mouseInteractions.setForceField(std::get<glm::vec2>(stateChange.value));
		break;
	case StateChangeType::Mouse_Burst:
		m_mouseInteractions.setBurst(std::get<glm::vec3>(stateChange.value));
		break;
	case StateChangeType::Mouse_SetAllRestPositions:
		m_particleSystem.applyAndRecord_SetAllRestPositions(std::get<glm::vec2>(stateChange.value), *this);
		break;
	default:
		assert(false && "[StateModifier::setApplyAndRecord] Forgot a case !");
	}
}

void StateModifier::apply() {
	if (!m_bBlockApplying)
		m_configManager.applyTo(m_particleSystem);
}

void StateModifier::recordChange(const StateChange& stateChange) {
	m_recordManager.recordChange(stateChange);
}

State StateModifier::getCurrentState() const {
	State state;
	state.timestamp                = m_recordManager.clock().time();
	state.lastShape                = m_configManager.getLastShapeAsActionRef();
	state.lastLayout               = m_configManager.getLastLayoutAsActionRef();
	state.currentAction            = m_configManager.getCurrentConfigAsActionRef();
	state.alphaTrailValues         = m_settingsManager.get().alphaTrail().getValues();
	state.colorValues              = m_settingsManager.get().colors().getValues();
	state.particleSystemValues     = m_settingsManager.get().partSystem().getValues();
	state.physicsValues            = m_settingsManager.get().physics().getValues();
	state.windValues               = m_settingsManager.get().wind().getValues();
	state.windDirectionAngle       = m_settingsManager.get().wind().getDirAngle();
	state.configParams             = m_configManager.configParams();
	state.randomParams             = m_configManager.randParams();
	state.textConfigValue          = m_configManager.textConfig().getText();
	state.alphaTrailPresetName     = m_settingsManager.get().alphaTrail().getPresetName();
	state.colorPresetName          = m_settingsManager.get().colors().getPresetName();
	state.particleSystemPresetName = m_settingsManager.get().partSystem().getPresetName();
	state.physicsPresetName        = m_settingsManager.get().physics().getPresetName();
	state.windPresetName           = m_settingsManager.get().wind().getPresetName();
	return state;
}

void StateModifier::setApplyAndRecord(const State& state, bool bSetInitialColorAndParticles) {
	m_configManager.setConfigParams(state.configParams);
	recordChange({ StateChangeType::Param, state.configParams });
	m_configManager.setRandParams(state.randomParams);
	recordChange({ StateChangeType::Random, state.randomParams });
	m_configManager.textConfig().setText(state.textConfigValue);
	recordChange({ StateChangeType::Text, state.textConfigValue });
	m_configManager.applyAndRecord_ActionRef(state.lastShape, *this);
	m_configManager.applyAndRecord_ActionRef(state.lastLayout, *this);
	m_configManager.applyAndRecord_ActionRef(state.currentAction, *this);
	if (bSetInitialColorAndParticles) {
		m_settingsManager.get().colors().setValues(state.colorValues);
		m_settingsManager.get().colors().setPresetName(state.colorPresetName);
		m_settingsManager.get().partSystem().setValues(state.particleSystemValues);
		m_settingsManager.get().partSystem().setPresetName(state.particleSystemPresetName);
	}
	m_settingsManager.get().alphaTrail().setValues(state.alphaTrailValues);
	m_settingsManager.get().alphaTrail().setPresetName(state.alphaTrailPresetName);
	m_settingsManager.get().physics().setValues(state.physicsValues);
	m_settingsManager.get().physics().setPresetName(state.physicsPresetName);
	m_settingsManager.get().wind().setValues(state.windValues);
	m_settingsManager.get().wind().setDirection(state.windDirectionAngle);
	m_settingsManager.get().wind().setPresetName(state.windPresetName);
	applyAndRecord_AllSettings();
}