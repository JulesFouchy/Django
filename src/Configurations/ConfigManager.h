#pragma once

#include "ConfigGPU.h"

#include "Helper/Array2D.h"

class ConfigManager {
public:
	ConfigManager();
	~ConfigManager() = default;

	inline ConfigGPU& get() { return m_shapeLayoutConfigs(m_currShapeIndex, m_currLayoutIndex); }

private:
	Array2D<ConfigGPU> m_shapeLayoutConfigs;
	size_t m_currShapeIndex = 1;
	size_t m_currLayoutIndex = 0;
};