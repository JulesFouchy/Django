#pragma once

#include "ConfigType.h"
#include "ConfigGPU.h"
#include "ConfigTextGPU.h"
#include "ConfigParams.h"
#include "RandomParams.h"
#include "ConfigSVGManager.h"
#include "Actions/KeyBindings.h"

#include "Helper/Array2D.h"

class StateModifier;

class ConfigManager {
public:
	ConfigManager();
	~ConfigManager() = default;

	void Imgui(StateModifier& stateModifier);
	inline void ImGuiKeyBindings(StateModifier& stateModifier) { m_keyBindings.ImGui(stateModifier); }

	void onWheel(int delta, bool bNoStandardScroll, StateModifier& stateModifier);
	void onKeyPressed(SDL_Scancode scancode, char keysym, StateModifier& stateModifier);
	inline void onKeyUp(SDL_Scancode scancode) { m_keyBindings.onKeyUp(scancode); }

	void applyAndRecord_ActionRef(const ActionRef& actionRef, StateModifier& stateModifier);

	inline ConfigParams& configParams() { return m_params; }
	inline RandomParams& randParams() { return m_randParams; }
	inline ConfigTextGPU& textConfig() { return m_textConfig; }
	inline void setConfigParams(const ConfigParams& params) { m_params = params; }
	inline void setRandParams(const RandomParams& randParams) { m_randParams = randParams; }

	inline void applyTo(ParticleSystem& partSystem) { get().applyTo(partSystem, m_params, m_randParams); }
	void applyAndRecord_Action(const Action& action, StateModifier& stateModifier);

	ActionRef getLastShapeAsActionRef();
	ActionRef getLastLayoutAsActionRef();
	ActionRef getCurrentConfigAsActionRef();

private:
	Configuration& get();
	void setCurrentConfigAsText();

private:
	ConfigTextGPU m_textConfig;
	Array2D<ConfigGPU> m_shapeLayoutConfigs;
	ConfigSVGManager m_svgManager;
	std::vector<ConfigGPU> m_standaloneConfigs;
	ConfigType m_currConfigType = ConfigType::SHAPE_LAYOUT;
	size_t m_currShapeIndex = 0;
	size_t m_currSvgIndex = 0;
	size_t m_currLayoutIndex = 0;
	size_t m_currStandaloneIndex = 0;
	ConfigParams m_params;
	RandomParams m_randParams;
	KeyBindings m_keyBindings;
	bool m_bLastShapeWasSVG = false; // TODO initialize this according to the startup config
};