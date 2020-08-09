#include "SettingsManager.h"

#include "Helper/File.h"

SettingsManager::SettingsManager()
	: m_currentSettings(), m_defaultSettings()
{
	if (MyFile::Exists(lastSessionFilePath())) {
		try {
			m_currentSettings.deserializeFrom(lastSessionFilePath());
		}
		catch (const std::exception& e) {
			spdlog::warn("lastSession.json is corrupted :\n {}",  e.what());
		}
	}
}

SettingsManager::~SettingsManager() {
	m_currentSettings.serializeTo(lastSessionFilePath());
}

std::string SettingsManager::settingsFolder() {
	return MyFile::RootDir + "/settings";
}
std::string SettingsManager::lastSessionFilePath() {
	return settingsFolder() + "/lastSessionSettings.json";
}