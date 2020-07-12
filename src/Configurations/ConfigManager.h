#pragma once

#include "ConfigGPU.h"

#include "Helper/Array2D.h"

class ParticlesSystem;

class ConfigManager {
public:
	ConfigManager();
	~ConfigManager() = default;

	inline ConfigGPU& get() { return m_shapeLayoutConfigs(m_currShapeIndex, m_currLayoutIndex); }

	void onKeyPressed(SDL_Scancode scancode, ParticlesSystem& partSystem);

private:
	Array2D<ConfigGPU> m_shapeLayoutConfigs;
	std::vector< ConfigGPU> m_standaloneConfigs;
	size_t m_currShapeIndex = 0;
	size_t m_currLayoutIndex = 0;
};