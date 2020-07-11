#pragma once

#include "ShapeLayoutConfig.h"

#include "Helper/Array2D.h"

class ConfigManager {
public:
	ConfigManager();
	~ConfigManager() = default;

	inline ShapeLayoutConfig& get() { return m_shapeLayoutConfigs(m_currShapeIndex, m_currLayoutIndex); }

private:
	Array2D<ShapeLayoutConfig> m_shapeLayoutConfigs;
	size_t m_currShapeIndex = 0;
	size_t m_currLayoutIndex = 0;
};