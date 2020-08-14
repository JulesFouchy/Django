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
class Recorder;

class ConfigManager {
public:
	ConfigManager();
	~ConfigManager() = default;

	inline void applyTo(ParticlesSystem& partSystem) { get().applyTo(partSystem, m_params, m_randParams); }
	void Imgui(ParticlesSystem& partSystem);
	inline void ImGuiKeyBindings(ParticlesSystem& partSystem, Recorder& recorder) { m_keyBindings.ImGui(*this, partSystem, recorder); }

	void onWheel(float delta, ParticlesSystem& partSystem, bool bNoStandardScroll);
	void onKeyPressed(SDL_Scancode scancode, char keysym, ParticlesSystem& partSystem, Recorder& recorder);
	inline void onKeyUp(SDL_Scancode scancode) { m_keyBindings.onKeyUp(scancode); }

	ConfigRef getCurrentConfigRef() const;

private:
	const Configuration& get() const;
	Configuration& get();
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