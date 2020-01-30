#pragma once

#include "Configurations.h"

class Config_FillScreen : public Configuration {
public:
	Config_FillScreen();
	~Config_FillScreen() = default;

	void embody(ParticlesSystem& particlesSystem) override;
	void generate() override;

private:
	int m_nbRows;
	int m_nbColumns;
};