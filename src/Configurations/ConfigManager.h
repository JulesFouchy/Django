#pragma once

#include "ConfigGPU.h"
#include "ConfigTextGPU.h"
#include "ConfigParams.h"
#include "RandomParams.h"
#include "ConfigSVGManager.h"
#include "ConfigType.h"
#include "Actions/KeyBindings.h"

#include "Helper/Array2D.h"

class ParticlesSystem;

class ConfigManager {
public:
	ConfigManager();
	~ConfigManager() = default;

	inline void applyTo(ParticlesSystem& partSystem) { get().applyTo(partSystem, m_params, m_randParams); }
	void Imgui(ParticlesSystem& partSystem);
	inline void ImGuiKeyBindings() { m_keyBindings.ImGui(); }

	void onWheel(float delta, ParticlesSystem& partSystem, bool bNoStandardScroll);
	void onKeyPressed(SDL_KeyboardEvent keyEvent, ParticlesSystem& partSystem);

private:
	Configuration& get();
	void setupKeyBindings();
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