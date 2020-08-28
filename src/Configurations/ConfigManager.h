#pragma once

#include "ConfigRef.h"
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

	void onWheel(float delta, ParticleSystem& partSystem, bool bNoStandardScroll);
	void onKeyPressed(SDL_Scancode scancode, char keysym, StateModifier& stateModifier);
	inline void onKeyUp(SDL_Scancode scancode) { m_keyBindings.onKeyUp(scancode); }

	ConfigRef getCurrentConfigRef();
	void applyActionRef(const ActionRef& actionRef, StateModifier& stateModifier);
	void applyConfigRef(const ConfigRef& configRef, StateModifier& stateModifier);

private:
	Configuration& get();
	void setCurrentConfigAsText();

friend class StateModifier;
	inline void applyTo(ParticleSystem& partSystem) { get().applyTo(partSystem, m_params, m_randParams); }
	void applyAction(const Action& action);

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