#include "Settings.h"

#include <cereal/archives/json.hpp>
#include <fstream>

Settings::Settings()
	: m_physicsSettings(), m_visualSettings(), m_windSettings()
{}

void Settings::ImGuiWindows(ShaderPipeline& physicsCompute) {
	ImGui::Begin("Style");
		m_visualSettings.ImGui();
	ImGui::End();
	ImGui::Begin("Wind");
		m_windSettings.ImGui(physicsCompute);
	ImGui::End();
	ImGui::Begin("Physics");
		m_physicsSettings.ImGui(physicsCompute);
	ImGui::End();
}

void Settings::apply(ShaderPipeline& physicsCompute) {
	m_visualSettings.apply();
	m_windSettings.apply(physicsCompute);
	m_physicsSettings.apply(physicsCompute);
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