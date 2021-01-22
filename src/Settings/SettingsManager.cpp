#include "SettingsManager.h"

#include "Helper/File.h"

SettingsManager::SettingsManager()
	: m_currentSettings(), m_defaultSettings()
{
	const std::string& path = Path::LastSession_Settings;
	if (MyFile::Exists(path)) {
		try {
			m_currentSettings.deserializeFrom(path);
		}
		catch (const std::exception& e) {
			spdlog::warn("lastSession.json is corrupted :\n {}",  e.what());
		}
	}
}

SettingsManager::~SettingsManager() {
	m_currentSettings.serializeTo(Path::LastSession_Settings);
}