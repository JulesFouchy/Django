#include "SettingsManager.h"

#include "Helper/File.h"

SettingsManager::SettingsManager()
	: m_currentSettings(), m_defaultSettings()
{
	if (MyFile::Exists(lastSessionFilePath())) {
		m_currentSettings.deserializeFrom(lastSessionFilePath());
	}
}

SettingsManager::~SettingsManager() {
	m_currentSettings.serializeTo(lastSessionFilePath());
}

std::string SettingsManager::settingsFolder() {
	return MyFile::RootDir + "/settings";
}
std::string SettingsManager::lastSessionFilePath() {
	return settingsFolder() + "/lastSession.json";
}