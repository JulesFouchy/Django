#pragma once

#include "ConfigGPU.h"
#include "ConfigParams.h"

#include "Helper/Array2D.h"

class ParticlesSystem;

enum class ConfigType {
	SHAPE_LAYOUT,
	STANDALONE
};

class ConfigManager {
public:
	ConfigManager();
	~ConfigManager() = default;

	inline void applyTo(ParticlesSystem& partSystem) { get().applyTo(partSystem, m_params); }
	Configuration& get();

	void onKeyPressed(SDL_Scancode scancode, ParticlesSystem& partSystem);
	void onWheel(float delta, ParticlesSystem& partSystem, bool bNoStandardScroll);

private:
	Array2D<ConfigGPU> m_shapeLayoutConfigs;
	std::vector<ConfigGPU> m_standaloneConfigs;
	ConfigType m_currConfigType = ConfigType::SHAPE_LAYOUT;
	size_t m_currShapeIndex = 0;
	size_t m_currLayoutIndex = 0;
	size_t m_currStandaloneIndex = 0;
	ConfigParams m_params;
};