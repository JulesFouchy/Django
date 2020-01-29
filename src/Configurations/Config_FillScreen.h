#pragma once

#include "Configurations.h"

class Config_FillScreen : public Configuration {
public:
	Config_FillScreen(ParticlesSystem& particlesSystem);
	~Config_FillScreen() = default;

	void embody() override;

private:
	void generate() override;

private:
	int m_nbRows;
	int m_nbColumns;
};