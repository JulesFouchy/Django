#pragma once

#include "ConfigGPU.h"
#include "ConfigTextGPU.h"
#include "ConfigParams.h"
#include "RandomParams.h"
#include "SVGssbo.h"

#include "Helper/Array2D.h"

class ParticlesSystem;

enum class ConfigType {
	SHAPE_LAYOUT,
	STANDALONE,
	TEXT
};

class ConfigManager {
public:
	ConfigManager();
	~ConfigManager() = default;

	inline void applyTo(ParticlesSystem& partSystem) { get().applyTo(partSystem, m_params, m_randParams); }
	void Imgui(ParticlesSystem& partSystem);
	Configuration& get();

	void onKeyPressed(SDL_KeyboardEvent keyEvent, ParticlesSystem& partSystem);
	void onWheel(float delta, ParticlesSystem& partSystem, bool bNoStandardScroll);

private:
	ConfigTextGPU m_textConfig;
	Array2D<ConfigGPU> m_shapeLayoutConfigs;
	std::vector<SVGssbo> m_svgSsbos;
	std::vector<ConfigGPU> m_standaloneConfigs;
	ConfigType m_currConfigType = ConfigType::SHAPE_LAYOUT;
	size_t m_currShapeIndex = 0;
	size_t m_currLayoutIndex = 0;
	size_t m_currStandaloneIndex = 0;
	ConfigParams m_params;
	RandomParams m_randParams;
};