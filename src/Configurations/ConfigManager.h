#pragma once

#include "ConfigRef.h"
#include "ConfigGPU.h"
#include "ConfigTextGPU.h"
#include "ConfigParams.h"
#include "RandomParams.h"
#include "ConfigSVGManager.h"
#include "Actions/KeyBindings.h"

#include "Helper/Array2D.h"

class ParticlesSystem;
class RecordManager;

class ConfigManager {
public:
	ConfigManager();
	~ConfigManager() = default;

	inline void applyTo(ParticlesSystem& partSystem) { get().applyTo(partSystem, m_params, m_randParams); }
	void Imgui(ParticlesSystem& partSystem);
	inline void ImGuiKeyBindings(ParticlesSystem& partSystem, RecordManager& recordManager) { m_keyBindings.ImGui(*this, partSystem, recordManager); }

	void onWheel(float delta, ParticlesSystem& partSystem, bool bNoStandardScroll);
	void onKeyPressed(SDL_Scancode scancode, char keysym, ParticlesSystem& partSystem, RecordManager& recordManager);
	inline void onKeyUp(SDL_Scancode scancode) { m_keyBindings.onKeyUp(scancode); }

	ConfigRef getCurrentConfigRef();
	void applyActionRef(const ActionRef& actionRef, RecordManager& recordManager);
	void applyConfigRef(const ConfigRef& configRef, RecordManager& recordManager);

private:
	Configuration& get();
	void applyAction(const Action& action, RecordManager& recordManager);
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