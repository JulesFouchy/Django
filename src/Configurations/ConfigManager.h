#pragma once

#include "ShapeLayoutConfig.h"

class ConfigManager {
public:
	ConfigManager();
	~ConfigManager() = default;

	inline ShapeLayoutConfig& get() { return m_config; }

private:
	ShapeLayoutConfig m_config;
};