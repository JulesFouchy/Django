#pragma once

#include "Settings.h"

class SettingsManager {
public:
	SettingsManager();
	~SettingsManager();

	inline Settings& get() { return m_currentSettings; }

private:
	static std::string settingsFolder();
	static std::string lastSessionFilePath();

private:
	Settings m_currentSettings;
	Settings m_defaultSettings;
};