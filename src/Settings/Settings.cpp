#include "Settings.h"

#include <cereal/archives/json.hpp>
#include <fstream>

Settings::Settings()
	: m_physicsSettings(), m_visualSettings(VisualSettings::DEFAULT), m_windSettings()
{}

void Settings::ImGuiWindows(ShaderPipeline& physicsCompute) {
	m_visualSettings.ImGui();
	ImGui::Begin("Wind");
	m_windSettings.ImGui_Parameters(physicsCompute);
	ImGui::End();
}

void Settings::serializeTo(const std::string& filePath) {
	std::ofstream os(filePath);
	{
		cereal::JSONOutputArchive archive(os);
		archive(
			CEREAL_NVP(m_physicsSettings),
			CEREAL_NVP(m_visualSettings),
			CEREAL_NVP(m_windSettings)
		);
	}
}

void Settings::deserializeFrom(const std::string& filePath) {
	std::ifstream is(filePath);
	{
		cereal::JSONInputArchive archive(is);
		archive(
			m_physicsSettings,
			m_visualSettings,
			m_windSettings
		);
	}
}