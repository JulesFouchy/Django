#pragma once

#include "ShapeLayoutConfig.h"

class ConfigManager {
public:
	ConfigManager();
	~ConfigManager() = default;
private:
	ShapeLayoutConfig m_config;
};