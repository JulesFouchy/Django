#pragma once

#include "ConfigGPU.h"

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

	ConfigGPU& get();

	void onKeyPressed(SDL_Scancode scancode, ParticlesSystem& partSystem);

private:
	Array2D<ConfigGPU> m_shapeLayoutConfigs;
	std::vector<ConfigGPU> m_standaloneConfigs;
	ConfigType m_currConfigType;
	size_t m_currShapeIndex = 0;
	size_t m_currLayoutIndex = 0;
	size_t m_currStandaloneIndex = 0;
};